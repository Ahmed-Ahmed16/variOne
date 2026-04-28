# VariOne — Agent Briefing

You are working on **VariOne**, a closed-source ESP32-based wireless security
awareness device. This file is the standing brief for every session.

## First thing, every session

Read `PRD.md` before doing anything else. It is the source of truth.
If anything you are about to do conflicts with the PRD, the PRD wins —
flag the conflict and ask before deviating.

Quick references inside the PRD:

- **Pin map:** §7.2 (single source of truth — firmware pin constants mirror it)
- **Module structure:** §8.2
- **Feature specs and acceptance criteria:** §9
- **Safety guards:** §5.4 and §9.5
- **Project plan and ownership:** §14
- **SD capture schemas:** §11

## Build system

- PlatformIO on VS Code.
- `platform = espressif32`, `board = esp32dev`, `framework = arduino`.
- Build with `pio run`. The operator flashes manually unless they explicitly
  ask for upload; do not assume an ESP32 is connected. Monitor at 115200.
- Library list lives in `platformio.ini`. Don't add libraries without
  saying so in the response.

## Hardware context (do not re-derive)

- ESP32-WROOM-32D N4 DevKit V1, dual-core CPU, single 2.4 GHz Wi-Fi
  radio, USB-C powered, 3.3 V rail from onboard LDO.
- SH1106 128×64 OLED on I²C (SDA=21, SCL=22).
- **PN532 NFC on the same I²C bus (SDA=21, SCL=22), address 0x24** —
  working configuration in current firmware. PN532 IRQ is physically
  wired to **GPIO 13** (firmware currently polls and does not subscribe
  to it — `Adafruit_PN532(255, 255)` — but the wire is there for future
  interrupt-driven detection). Code also defines `PIN_NFC_RST = 13` /
  `PIN_NFC_SS = 27` from a prior SPI plan; these are unused in I²C mode
  but kept as no-ops. Do **not** switch PN532 to SPI mode without
  explicit instruction.
- CC1101 + microSD share VSPI (SCK=18, MISO=19, MOSI=23) with separate
  CS lines. Every transaction must hold the SPI bus mutex defined in
  `src/core/SpiBus.*`. Current working wiring uses CC1101 CSN=GPIO 15,
  CC1101 GDO0=GPIO 4, and SD CS=GPIO 5.
- CC1101 is **3.3 V only** on VCC. Never suggest 5 V on the CC1101.
- 4 buttons, all `INPUT_PULLUP`. Working pinout: LEFT=14, UP=26,
  RIGHT=32, DOWN=33. Treat menu navigation as 4-way (UP/DOWN scroll,
  RIGHT=select/enter, LEFT=back/exit) until PRD §7.2 reconciliation
  decides otherwise.
- IR RX on GPIO 36 (VS1838B, currently wired). IR TX on GPIO 25 (planned;
  38 kHz LEDC carrier, NPN driver).
- Sub-GHz front end: optional **external LNA + 433.92 MHz SAW filter**
  in front of CC1101 to reject sensor/mains/ISM noise so the radio
  focuses on car-key fobs and garage gates. Spare CC1101 module is on
  hand; second module reserved for future TX/RX split if needed.
- **Power**: USB-C from bench/wall during development. Final demo target
  is **standalone battery operation, USB-C rechargeable** — single
  protected 18650 (~3000 mAh, preferred) **or** slim LiPo pouch
  (1500–2000 mAh, fallback) + **USB-C TP4056-with-protection** charger
  + MT3608 boost to 5 V → ESP32 VIN. See PRD §6.4 for full architecture
  and rules. CC1101 stays 3.3 V only; 5 V boost feeds ESP32 VIN, onboard
  LDO produces the 3.3 V rail for every peripheral. Add hard-off slide
  switch, 100 k/100 k ADC divider on GPIO 35 for low-battery cutoff,
  and a 1000 µF + 100 nF bulk cap across the boost output to suppress
  TX-burst sag.

## Coding conventions

- One C++ class per peripheral, in its own `.h/.cpp` pair under the
  directory tree in PRD §8.2 / Appendix A.
- Every file starts with a one-paragraph header comment stating its
  responsibility and which PRD section it implements.
- No global state outside `src/config.h`. Pass dependencies in
  constructors.
- Long-running operations are non-blocking and yield to the input handler
  so BACK can always cancel.
- Capture files on SD use the schemas in PRD §11. Don't invent new
  schemas without updating the PRD.
- Comments in English. Variable names in English.

## How to take a task

When asked to implement a feature:

1. State which PRD section governs it.
2. Restate the acceptance criteria from §9 verbatim.
3. List the files you will create or change.
4. Implement.
5. Note which acceptance criteria are now testable on hardware and which
   still depend on later modules.

If the request is vague, ask which PRD acceptance criterion it maps to
before writing code.

## Sensor-first test discipline (mandatory for any peripheral not yet
working in `main.cpp`)

Before integrating any new sensor, RF front end, or bus device into the
main firmware, you **must** ship a standalone serial-only test sketch
under `test/test_<sensor>.cpp` (or a dedicated PlatformIO env in
`platformio.ini`) that:

1. Initializes only that one peripheral plus serial.
2. Prints a known-good signature on boot (chip ID, firmware version,
   register dump, etc.) so a missing/miswired part is obvious.
3. Loops a primitive sense/transmit operation with serial output every
   1–2 s.
4. Runs cleanly with **no edits to `main.cpp`** and no risk of breaking
   the v0.4 working feature set.

Only after the smoke test prints the expected signature on hardware do
you integrate the peripheral into `main.cpp`. Document the smoke-test
result (pass/fail, observed values, date) in `docs/test-log.md` before
merging integration code. This rule exists because the CC1101 bring-up
already broke the OLED once on shared SPI — see `CLAUDE.md` (legacy)
"Current blocker" notes.

Inconsistencies between sensors (bus contention, brownout, CS hygiene,
shared-pin conflicts) must be caught at the smoke-test stage, not after
integration.

## UI / mascot rule (every screen)

Every user-facing screen and every interaction wires a mascot mood. The
mascot is a first-class UX element, not decoration:

- **On screen entry:** set an idle mood appropriate to the screen
  (`THINKING` for menus, `WORKING` for active scans, `WAVING` while a
  scan is in flight).
- **On result:** transition to `HAPPY` / `SUCCESS` on a positive event
  (AP found, card read, signal captured, replay accepted),
  `SAD` / `FAIL` on a negative one (no AP, no card, no signal, replay
  rejected), `ANGRY` on attack-class start (deauth burst, replay TX).
- **Available moods:** IDLE, HAPPY, THINKING, SAD, ANGRY, SLEEPING,
  SUCCESS, FAIL, WORKING, WAVING (preserve from v0.4).

Full mood animations and gamification (score, streaks, screen-corner
mini-mascot) are **lower priority than sensor work** — stub the mood
call (`triggerReaction(MOOD_X, ...)`) on every screen now, polish
animations later. Do not ship a screen with no mascot mood call.

## WiFi attack reference — ESP32 Marauder

For Wi-Fi feature development (F4/F5/F6) consult ESP32 Marauder
(`https://github.com/justcallmekoko/ESP32Marauder`) as a reference for
known-working ESP32 Wi-Fi attack code paths (deauth frame structure,
beacon spam, evil-twin SoftAP setup, channel-hop timing). **Study,
don't copy** — VariOne is closed source and Marauder is GPL. Port
*behavior and constraints*, not source files. Note any Marauder
technique you adopt in the file header comment plus a one-liner in
`docs/test-log.md`.

## Wi-Fi revamp status (active)

Treat the current Wi-Fi feature set as **not complete and not trusted**.
The old v0.4 Wi-Fi screens are allowed to compile, but F4/F5/F6 need a
real revamp before demo readiness:

- Rebuild AP scan, station/client discovery, selected-target state,
  deauth, evil twin, and portal as one coherent workflow.
- Do not preserve old Wi-Fi behavior just because it exists in
  `main.cpp`; preserve only behavior that still matches the PRD and works
  on hardware.
- Keep the hard safety rails: no broadcast deauth, no unselected station
  target, no deauth and evil twin at the same time, no real-world brand
  impersonation.
- The all-client demo path is allowed only as **All discovered clients**:
  after selecting a lab-owned AP and running station discovery, the
  firmware may loop over those discovered station MACs and send individual
  unicast frames. It must never fall back to `FF:FF:FF:FF:FF:FF`.
- Portal rebrand testing is allowed only through generic lab/demo themes.
  Theme text/colors may change; real bank, ISP, university, telco, or
  platform names/logos are not allowed.
- ESP32-WROOM-32D N4 is dual-core, but Wi-Fi constraints here are radio
  constraints, not single-core CPU constraints.

## Hard rules — never violate

- **Never** broadcast deauth (`FF:FF:FF:FF:FF:FF`). "All clients" means
  all discovered stations under the selected lab BSSID, sent as individual
  unicast frames only. Do not add a flag to enable broadcast fallback.
- **Never** run deauth and the rogue AP simultaneously (PRD §9.6).
- **Never** target a station or BSSID that the operator has not
  explicitly selected from the F4 scan results.
- **Never** impersonate a specific real-world brand in the captive
  portal HTML (no bank, ISP, telco, or platform names or logos). The
  page stays generic.
- **Never** write an unmasked PAN to the SD card or to the OLED. Mask
  to last-4 (`**** **** **** 1234`) at the parser, not at the display
  layer.
- **Never** transmit captured data off-board over the network.
- **Never** add features outside the PRD scope without an explicit
  request and a PRD update in the same change.

## Demo context (so you understand why constraints exist)

The device is a graduation project for CIC New Cairo, supervised by
Dr. Ahmed Gaber, operated only inside three formally authorized test
environments. The audience is non-technical. The goal is awareness,
not exploitation. Every safety guard above exists because a reviewer
will ask about it.

## When in doubt

Prefer the smaller, safer change. Prefer asking over guessing. Prefer
matching the PRD over improving on it.

## Pin-map reconciliation note (open)

The PRD §7.2 pin map has been reconciled to the current working firmware:
CC1101 CSN=15, GDO0=4, PN532 on I²C, buttons LEFT=14/UP=26/RIGHT=32/DOWN=33,
IR RX=36, IR TX=25 planned. The **working firmware wins** if a future doc
drifts again. Do not move a working pin to satisfy an old note — flag the
conflict and update the PRD instead.
