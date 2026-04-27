# VariOne — Task Tracker

> Living document. Tracks every task from repo bootstrap to demo day.
> Mirrors PRD §13.2 (bring-up checklist), §14.2 (10-day plan), and §9
> (feature acceptance criteria).

---

## How to use this file

- A task is **done** only when its checkbox is ticked AND, where listed, its
  acceptance criterion has been verified on hardware.
- **Coding tasks:** Claude Code ticks the box in the same session it
  completes the work, and adds a one-line note `(YYYY-MM-DD, commit <sha7>)`
  next to the box.
- **Hardware tasks:** ticked manually by the owner with initials,
  e.g. `- [x] Wire OLED I²C  (A.M.)`.
- Daily standup format: each engineer reads their next 3 unchecked items.
- If a task slips, **do not delete it** — move it down a day and add a note.
- If a task is cut to save time, mark it `- [~]` and add `(cut, see RK#)`.

### Status legend

- `- [ ]` not started
- `- [/]` in progress
- `- [x]` done and verified
- `- [~]` deliberately cut (with reason)
- `- [!]` blocked (see "Blockers" at bottom)

### Owners (PRD §14.1)

- **A** — Hardware/RF lead (wiring, PCB, antennas, CC1101)
- **B** — Firmware core (loop, SPI mutex, SD, OLED, menu, input)
- **C** — NFC (PN532 + EMV-lite parser)
- **D** — Wi-Fi (scan, deauth, evil twin, portal)
- **E** — IR + Sub-GHz codec + integration testing + documentation

---

## Phase 0 — Repo bootstrap (before Day 1)

Owner: B, with help from anyone available.

- [ ] Create private git repo `varione`
- [ ] Drop `CLAUDE.md` at repo root
- [ ] Drop `PRD.md`
- [ ] Drop `tasktracker.md` (this file)
- [ ] **Reconcile PRD §7.2 pin map against working v0.4 firmware** — `src/main.cpp` is the source of truth for what is wired and known good; PRD must match (PN532 on I²C, CC1101 CSN=15, GDO0=4, buttons LEFT=14/UP=26/RIGHT=32/DOWN=33, IR RX=36, IR TX=25 planned). Update Appendix B `config.h` accordingly.
- [ ] Create `platformio.ini` per PRD §8.1, **plus per-peripheral test envs** (`[env:cc1101_test]`, `[env:pn532_test]`, `[env:sd_test]`, `[env:ir_test]`)
- [ ] Create `src/config.h` from PRD Appendix B (post-reconcile)
- [ ] Keep current working `src/main.cpp` (v0.4) as the integration target; do **not** stub-overwrite
- [ ] First green `pio run` on the main env (working firmware still compiles)
- [ ] Push to private remote, all 5 engineers cloned and building locally
- [ ] `docs/diary/` directory created (one daily 5-line note per engineer)
- [ ] `docs/test-log.md` created — one row per smoke test, columns `date | peripheral | env | signature | result | notes`
- [ ] `docs/rf-frontend.md` created — RF chain decisions per PRD §7.4

**Gate:** every team member can `pio run` cleanly before Day 1 starts, working v0.4 features still run on hardware after merge.

---

## Active Wi-Fi Revamp Track

Owner: D, with B for firmware state-machine integration.

Status: Wi-Fi is **not fixed**. Treat F4/F5/F6 as active implementation
work, not polish.

- [/] Replace old AP/station targeting flow with a coherent selected-target model
- [x] Re-include `All discovered clients` as a target-bound option that loops over scanned station MACs; never broadcast to `FF:FF:FF:FF:FF:FF`
- [/] Harden station discovery: show station MAC, RSSI, frame count, and target AP channel
- [x] Make deauth bursts time-bound, cancelable, target-bound, and visibly counted down
- [x] Make evil twin start/stop cleanly from a selected AP and never overlap with deauth
- [x] Add portal rebrand test themes that stay generic and non-impersonating
- [x] Keep captive portal generic and write masked demo credentials to `/captures/portal.log`
- [x] Add serial debug output for AP scan, station discovery, deauth target mode, portal theme, and stop reasons
- [x] Add cold-open wow factor screen: mascot, firmware identity, dual-core ESP32 note, RF/NFC/Wi-Fi/IR ready line
- [ ] Hardware test F4 scan against lab AP and log AP count/time
- [ ] Hardware test F5 against lab WPA2/PMF-off AP and log phone-drop timing
- [ ] Hardware test F6 portal redirect on lab phone and log redirect time

**Gate:** S4 runs end-to-end as `scan → select AP → discover station →
select station/all discovered → deauth burst stops → start evil twin →
portal submission`, with no broadcast deauth and no real-brand portal assets.

---

## Day 1 — Hardware bring-up & project skeleton

References: PRD §13.2 steps 1–3 + 6, §14.2 row Day 1.

**Status note.** v0.4 firmware already covers a large subset of Day 1 (OLED splash + mascot, 4-button input, WiFi scan, NFC UID + EMV AID, IR RX on GPIO 36). Day 1 is therefore **verification + smoke-test scaffolding**, not bring-up from scratch.

### A — Hardware
- [x] Wire ESP32 power rail, verify 3.3 V on rail with multimeter (v0.4)
- [x] Wire OLED I²C: SDA→GPIO21, SCL→GPIO22, VCC→3.3V, GND (v0.4)
- [x] Wire 4 buttons to GPIO 14/26/32/33, common GND, INPUT_PULLUP (v0.4)
- [ ] Re-wire CC1101 carefully: VCC→**3.3 V only**, GND, SCK=18, MISO=19, MOSI=23, CSN=15, GDO0=4, GDO2 unconnected. Multimeter-verify 3.3 V on VCC pin; press every jumper firmly; verify no pins shorted to neighbors.
- [x] CC1101 antenna whip already attached (E07-M1101D V2.0)
- [ ] Visual continuity check vs reconciled PRD §7.2

### B — Core firmware
- [x] PlatformIO project compiles, v0.4 features run (`pio run`)
- [ ] Add per-peripheral PlatformIO test envs (cc1101_test, pn532_test, sd_test, ir_test)
- [ ] `src/core/Logger.{h,cpp}` — extract serial logger from main.cpp into module (refactor — do not regress)
- [ ] `src/ui/Display.{h,cpp}` — extract U8g2 SH1106 wrapper into module
- [ ] `src/ui/Input.{h,cpp}` — extract 4-button debounce into module, expose LEFT/UP/RIGHT/DOWN events
- [x] Bring-up §13.2 step 1: boot banner prints on serial (v0.4)
- [x] Bring-up §13.2 step 2: OLED renders splash for 1.5 s (v0.4)
- [x] Bring-up §13.2 step 3: all 4 buttons report distinct presses (v0.4)

### C — NFC (PN532 on I²C)
- [x] Confirm PN532 DIP switches in **I²C mode** (SEL0=1, SEL1=0) (v0.4)
- [x] Wire PN532: VCC, GND, SDA=21, SCL=22 shared with OLED, IRQ=GPIO 13 (v0.4)
- [x] PN532 detected on I²C scan at 0x24 (v0.4)
- [x] First UID read from ISO 14443A card prints to serial (v0.4)
- [ ] (Optional / later) switch from poll to IRQ-driven detection — wire is on GPIO 13, change `Adafruit_PN532(255, 255)` to `Adafruit_PN532(13, /*RST=*/ -1)` and attach an `attachInterrupt`
- [ ] **Smoke test §13.0:** `test_pn532` env runs an I²C scan, prints PN532 firmware version, polls UID — confirm bus shared with OLED still draws cleanly during NFC poll
- [ ] `src/nfc/NfcReader.{h,cpp}` — extract NFC code from main.cpp into module

### D — Wi-Fi
- [x] `WiFi.scanNetworks()` wrapper, AP list to OLED + serial (v0.4)
- [x] ≥5 APs detected in lab within 5 s (v0.4)
- [ ] `src/wifi/WifiScan.{h,cpp}` — extract from main.cpp into module
- [ ] Cross-reference Marauder for any AP/station enumeration trick we are missing; note adopted ideas in module header

### E — IR / docs
- [x] Wire VS1838B to GPIO 36 (v0.4)
- [ ] **Smoke test §13.0:** `test_ir` env decodes ≥3 different remote presses on serial, before any IR module integration
- [ ] `src/ir/IrRx.{h,cpp}` — IRremoteESP8266 RX module (post-smoke-test only)
- [ ] Document protocols seen in `docs/ir-protocols.md` (NEC / Sony / RC5 / RC6 / etc.)

### Mascot wiring (cross-cutting, B+E)
- [x] v0.4 mascot system in place (10 moods, boot animation, key-event reactions)
- [ ] Audit every screen in current `main.cpp`; confirm a `triggerReaction(MOOD_X, ...)` call exists on entry **and** on each state transition. List any screens missing a mood call in `docs/mascot-audit.md`.

**Day 1 gate:** v0.4 features verified intact on hardware after refactor; CC1101 re-wired and ready for Day 2 smoke test; PN532 + IR smoke tests have a green entry in `docs/test-log.md`; every screen has a mascot mood call.

---

## Day 2 — Storage, capture API, menu skeleton

References: PRD §11 (capture schemas), §14.2 row Day 2.

### A — Hardware
- [ ] Wire microSD module: VCC, GND, SCK/MISO/MOSI shared with VSPI, CS=5. If SmartRC issue #40 bites, re-plan the alternate bus against the current pin map first; the old HSPI fallback conflicts with working pins.
- [ ] Insert FAT32-formatted card (8–32 GB Class 10), verify card not write-protected
- [ ] **Smoke test §13.0:** `test_sd` env mounts the card, prints free MB, lists `/`. Run with CC1101 powered but unselected to verify CS hygiene.

### B — Core firmware
- [ ] `src/core/SpiBus.{h,cpp}` — mutex + per-peripheral `SPISettings` switcher
- [ ] `src/storage/SdCard.{h,cpp}` — mount, free-space, list directory
- [ ] `src/storage/CaptureStore.{h,cpp}` — write/read JSON captures per PRD §11
- [ ] `src/ui/MenuTree.{h,cpp}` — static menu definition (5 top-level entries)
- [ ] `src/ui/Screens.{h,cpp}` — list / action / confirm screen primitives
- [ ] **Bring-up §13.2 step 4:** SD card mounts, free MB prints to serial
- [ ] All SPI peripheral CS lines initialized HIGH at boot before any driver runs

### C — NFC
- [ ] `EmvLite.{h,cpp}` skeleton: `SELECT PPSE` + parse FCI → AID
- [ ] Test against one EMV test card; AID logged to serial
- [ ] No PAN reads yet — parser scaffolding only

### D — Wi-Fi
- [ ] Promiscuous-mode packet sniffer, channel-hop 1–13
- [ ] Station scan: enumerate `(BSSID, station MAC, channel, RSSI)` tuples
- [ ] Live count of unique stations prints to serial

### E — IR + Sub-GHz
- [ ] `src/ir/IrTx.{h,cpp}` — 38 kHz LEDC carrier on GPIO 25
- [ ] NEC protocol round-trip: capture from remote, retransmit, target device responds
- [ ] Wire IR LED via NPN driver (2N3904 + 100 Ω + base resistor 1 kΩ)

**Day 2 gate:** SD mounts, menu draws, NFC AID parsed, station scan runs, IR round-trips.

---

## Day 3 — Menu polish, EMV parsing, deauth, sub-GHz scaffolding

### A — Hardware
- [ ] Enclosure CAD v1 finalized
- [ ] Send enclosure to 3D printer (lead time check — pull this earlier if needed)

### B — Core firmware
- [ ] Menu navigation complete: UP/DOWN scroll, SELECT enter, BACK exit, breadcrumb header
- [ ] Settings page: brightness, default sub-GHz freq, deauth timeout, demo mode
- [ ] Settings persist to `/config.json` on SD

### C — NFC
- [ ] `EmvLite` extended: `SELECT AID` → GPO → `READ RECORD`
- [ ] Parse Track 2 equivalent → PAN, expiry, name (where present)
- [ ] **PAN masked at parser** to last-4 (`**** **** **** 1234`) — never store full PAN
- [ ] Capture written to `/captures/nfc/<timestamp>.json` per PRD §11.2

### D — Wi-Fi
- [ ] `src/wifi/Deauth.{h,cpp}` — craft 802.11 deauth frame (reason 7)
- [ ] `esp_wifi_80211_tx()` injection on target channel
- [ ] **No targeting yet** — verify frame structure on a packet capture (Wireshark on lab AP)

### E — Sub-GHz
- [ ] **Smoke test §13.0:** `test_cc1101` env reads PARTNUM=0x00 / VERSION=0x14 (or 0x04). Hold OLED off (do not init U8g2) on first run to isolate SPI hygiene; then re-run with OLED active to prove the v0.4 OLED-vs-CC1101 conflict is fixed. Log result in `docs/test-log.md`.
- [ ] `src/radio/SubGhz.{h,cpp}` — CC1101 init, register dump (only after smoke test green)
- [ ] **Bring-up §13.2 step 5:** PARTNUM=0x00 / VERSION=0x14 read confirms CC1101 alive **inside main.cpp** with all v0.4 features still working
- [ ] `src/radio/SubGhzCodec.{h,cpp}` — OOK edge encode/decode skeleton
- [ ] Mascot mood call wired in Sub-GHz screen (`THINKING` on entry, `WORKING` on listen, `HAPPY` on edges, `SAD` on timeout)

**Day 3 gate:** menu fully navigable, NFC writes masked PAN to SD, deauth frame visible in Wireshark, CC1101 verified alive on isolated test **and** integrated with OLED active.

---

## Day 4 — Safety guards, fit check, sub-GHz capture

### A — Hardware
- [ ] Enclosure print received, fit check ESP32 + modules
- [ ] Antenna mount external (CC1101 whip should be outside the case)
- [ ] Document any rework needed for v2

### B — Core firmware
- [ ] `src/core/StatusLed.{h,cpp}` — slow blink idle, fast blink active, solid TX
- [ ] Error toast UI on OLED (3-second flash, dismissible with BACK)
- [ ] Confirm-screen primitive (SELECT-hold 1 s for destructive actions)

### C — NFC
- [ ] NFC capture file format finalized — schema version 1 frozen
- [ ] SHA-1 of payload computed and written into capture file
- [ ] Validation: capture, reboot, re-read file, schema and SHA-1 verify

### D — Wi-Fi
- [ ] Deauth **target-binding**: only accept BSSID + station MAC selected from F4 scan
- [ ] **Hard guard:** broadcast target (`FF:FF:FF:FF:FF:FF`) rejected with on-screen error
- [ ] Time-bound burst: default 15 s, max 60 s, halts cleanly
- [ ] Radio returns to scan-ready state after halt

### E — Sub-GHz
- [ ] CC1101 OOK/ASK config at 433.92 MHz, ~2.4 kbps
- [ ] **Tighten RX bandwidth register** to 58 kHz (PRD §7.4 option 1) to suppress sensor / mains noise; baseline RSSI floor measurement before/after, log in `docs/rf-frontend.md`
- [ ] Capture path: GDO0 timing → circular edge buffer → end-detect on inter-edge timeout
- [ ] Bench test: capture from a known fixed-code remote, edges array makes sense
- [ ] **Decision point — RF front end (PRD §7.4):** if bench capture is unreliable, order SAW filter (option 2) and/or LNA (option 3). Record decision + measurement in `docs/rf-frontend.md`. Egypt-source vendors listed in PRD §7.4 BOM notes.
- [ ] Mascot moods wired: `WORKING` on listen, `HAPPY` on capture, `SAD` on timeout, `ANGRY` on TX

**Day 4 gate:** F5 (deauth) acceptance criteria AC1+AC3 testable; sub-GHz captures from a bench remote with the chosen RX bandwidth setting.

---

## Day 5 — Logging, rogue AP, FIAT bench test

### B — Core firmware
- [ ] Logger writes to `/system.log` on SD with rotation at 1 MB
- [ ] Free RAM and loop latency printed every 30 s in debug builds

### C — NFC
- [ ] NFC feature complete; AC1+AC2 from PRD §9.2 verified
- [ ] Brief written for owner E so sub-GHz capture schema follows the same shape

### D — Wi-Fi
- [ ] `src/wifi/EvilTwin.{h,cpp}` — SoftAP open mode, cloned SSID from F4 selection
- [ ] Same channel as target AP (no channel-hop while SoftAP is up)
- [ ] Lab phone can manually associate to cloned SSID

### E — Sub-GHz
- [ ] Replay path: load capture, retransmit edges via CC1101 TX
- [ ] **Bench test:** capture FIAT 128 unlock signal in quiet RF environment
- [ ] Replay against FIAT — door unlocks at ≤5 m line-of-sight (PRD §9.1 AC1)
- [ ] Record number of successful unlocks out of 5 attempts in `docs/test-log.md`
- [ ] **Garage-gate target (PRD §9.1 AC4):** identify a lab-/team-accessible fixed-code garage remote (PT2262 / EV1527), capture, replay, log success rate at ≤10 m
- [ ] **Modern-vehicle comparison set (PRD §9.1.1) — start data collection:**
  - [ ] Mini Cooper 2016 (Dr. Gaber): capture fob press, attempt replay at ≤2 m, log result + observed modulation/freq/edges. Expected: rejected.
  - [ ] JAC S3 2018: same. Expected: rejected.
  - [ ] Mercedes E180: same. Expected: rejected (likely Hitag2 or 868 MHz).
  - [ ] Avatr: same. Expected: no usable 433 MHz OOK at all (smart-key / UWB).
  - [ ] All five rows committed to `docs/modern-car-comparison.md` with frequency, modulation, edges-per-press, edges-differ-across-presses?, replay outcome.

**Day 5 gate:** F1 AC1+AC2+AC3 verified on the actual FIAT; ≥1 garage gate captured + replayed; ≥3 modern-car negative-result rows logged; rogue AP broadcasts a cloned SSID.

---

## Day 6 — Captive portal, performance, viva prep

### B — Core firmware
- [ ] Performance pass: free RAM ≥80 KB at idle, loop latency ≤5 ms at idle
- [ ] Memory-leak check: 1-hour idle run, RAM stable

### C — NFC
- [ ] Hardening: malformed cards / missing AID handled without crash, falls back to UID display
- [ ] Viva-prep note in `docs/viva-prep.md`: §9.2 reality-check answer drilled by every team member

### D — Wi-Fi
- [ ] `src/wifi/Portal.{h,cpp}` — DNS hijack (resolve all to ESP32 IP)
- [ ] HTTP server serves generic router-style login page (PROGMEM HTML/CSS)
- [ ] **No real-world brand impersonation** — confirm with peer review on the page
- [ ] Lab phone connecting to cloned SSID is redirected to portal within 10 s (§9.6 AC1)

### E — Sub-GHz / FIAT / modern cars
- [ ] Live FIAT test in lab parking — full demo rehearsal of S1
- [ ] Document range: meters at which replay still works
- [ ] Note any RF interference in the parking environment
- [ ] Complete remaining modern-car rows in `docs/modern-car-comparison.md`
- [ ] Build a single OLED screen (`Sub-GHz → Compare`) that reads from the comparison file and shows one row at a time — used as the on-stage prop for the §9.1.1 negative-result narrative

**Day 6 gate:** F6 AC1 verified; FIAT demo runs reliably outside of the bench.

---

## Day 7 — Final assembly, portal capture, IR presets

### A — Hardware
- [ ] Final enclosure assembly: ESP32, modules, OLED, buttons mounted
- [ ] Strain relief on all external wires (USB-C, antenna)
- [ ] Cosmetic pass — labels for buttons, status LED visible
- [ ] **Battery integration decision (PRD §6.4):** pick architecture A (18650, default) or B (LiPo pouch) based on enclosure room. Bench-test boost rail under worst-case TX (Wi-Fi deauth + OLED + scan = ~600 mA). Cell voltage must stay ≥3.5 V during burst (RK13).
- [ ] Buy parts (Cairo): protected 18650 (Samsung 30Q / Sony VTC6 / Panasonic NCR18650B) **or** 103450 LiPo + USB-C TP4056-protected + MT3608 + cell holder + 1000 µF cap + 100 nF cap + SPST switch + 2×100 kΩ
- [ ] Solder USB-C TP4056-protected + MT3608 boost. Trim MT3608 pot to **5.0 V exactly**, then epoxy.
- [ ] Verify 5.0 ± 0.1 V on boost output under load (dummy 600 mA)
- [ ] SPST hard-off switch wired between cell + and boost input
- [ ] 1000 µF + 100 nF across boost output, mounted close to ESP32 VIN
- [ ] ADC voltage divider (100 k / 100 k) on **GPIO 35** (input-only ADC1); firmware reads cell voltage every 5 s
- [ ] Low-battery mascot mood wired: `SAD` badge below ~3.6 V (~20%), `FAIL` cutoff below ~3.3 V — radios halted
- [ ] USB-C jack from TP4056 module exposed through enclosure; ≥5 mm clearance + ventilation slot above charge module
- [ ] **Battery-only test:** unplug USB-C, run S1+S2+S3+S4 once on battery — log runtime + final cell voltage in `docs/test-log.md`

### B — Core firmware
- [ ] **Integration freeze for menu** — no menu changes after end of Day 7
- [ ] Demo-mode toggle in settings (hides debug overlays during live demo)

### D — Wi-Fi
- [x] Portal POST handler writes masked demo submissions to `/captures/portal.log`
- [x] On-screen masked display: user/demo count only, password never shown raw
- [ ] §9.6 AC2+AC3 verified
- [ ] **Hard guard re-check:** deauth and evil twin cannot run simultaneously (UI state machine forbids it)

### E — IR
- [ ] Universal-remote presets bundled (TV power, AC on/off, projector) for at least 3 brands
- [ ] §9.3 AC1+AC2 verified

**Day 7 gate:** all six features (F1–F6) meet their acceptance criteria. Feature freeze. Days 8–10 are stabilization only.

---

## Day 8 — Dry run #1

All engineers participate.

- [ ] Full S1 (FIAT unlock) end to end, on enclosure, scripted narration
- [ ] Full S2 (bank card read) end to end with operator's own card
- [ ] Full S3 (IR universal remote) end to end on lab AC + lab TV
- [ ] Full S4 (deauth → stop → rogue AP → portal) end to end on lab AP + lab phone
- [ ] Cold-boot recovery: power-cycle device mid-demo, ensure clean recovery
- [ ] Capture replay-after-reboot: capture on Day 8 morning, replay Day 8 afternoon
- [ ] Fix list compiled in `docs/dryrun-day8.md`
- [ ] All P1 fixes from list resolved by end of day

**Day 8 gate:** all four scenarios run successfully without operator intervention beyond menu presses.

---

## Day 9 — Dry run #2 with external observer

- [ ] Recruit one non-team member to watch (a friend from the program is fine)
- [ ] Run S1–S4 in front of them, no rehearsing the script with them in earshot first
- [ ] Collect observer feedback on:
  - Pacing (each scenario ≤3 minutes)
  - Comprehensibility for non-technical audience
  - Visual moments that landed vs. fell flat
- [ ] P1 fixes only — no new features
- [ ] **Doctrine drill:** every team member answers the §9.2 reality-check question (bank cards) in ≤60 s
- [ ] **Doctrine drill:** every team member can explain §9.6 sequencing (why deauth and rogue AP run separately)
- [ ] Backup SD card pre-loaded with all captures, kept in kit
- [ ] Spare USB-C cable + spare antenna in kit
- [ ] Spare charged 18650 (or USB power bank as architecture-C fallback per PRD §6.4) in kit
- [ ] Battery runtime measured end-to-end on Day 9 battery rehearsal

**Day 9 gate:** non-technical observer understood at least 3 of 4 scenarios without prompting.

---

## Day 10 — Demo day

### Morning (A + everyone)
- [ ] Demo logistics: cabling, projector connection, phone for S4, FIAT in parking spot
- [ ] Smoke test S1–S4 once in the actual demo room
- [ ] Charge / verify all peripheral devices (lab phone, FIAT key for comparison)

### Demo
- [ ] Live demo S1–S4
- [ ] Q&A and viva
- [ ] Hand over `docs/` package (PRD, test log, viva-prep, this tracker) to supervisor

### Post-demo
- [ ] Wipe `/captures/portal.log` on SD
- [ ] Final commit tagged `v1.0.0-demo`
- [ ] Retrospective note in `docs/retro.md`

---

## Demo readiness gates (cross-cutting acceptance)

These are the must-pass gates from PRD §16. Tick when verified end-to-end.

- [ ] **S1 FIAT unlock** — replay unlocks doors in ≥4 of 5 attempts at ≤5 m
- [ ] **S1b Garage gate** — replay opens lab/team gate in ≥3 of 5 attempts at ≤10 m (PRD §9.1 AC4)
- [ ] **S1c Modern-car comparison** — at least 3 modern-vehicle negative-result rows on stage (Mini, JAC, Mercedes, Avatr) with on-OLED narration (PRD §9.1 AC5 + §9.1.1)
- [ ] **S2 Bank card read** — masked PAN + expiry on OLED within 2 s at ≥2 cm
- [ ] **S3 IR replay** — TV/AC powers on at ≥3 m
- [ ] **S4a Deauth** — lab phone drops Wi-Fi within 5 s, halts at timeout
- [ ] **S4b Evil twin + portal** — phone redirected to portal within 10 s, demo submissions masked on-screen and written to SD
- [ ] **Persistence** — capture, hard reboot, replay still works
- [ ] **Mascot coverage** — `docs/mascot-audit.md` shows zero screens without a mood call
- [ ] **Standalone battery** — full S1–S4 cycle runs on battery only with cell ≥3.5 V at end (PRD §6.4)
- [ ] **Doctrine** — every team member can deliver the §9.2 reality-check answer

---

## Risk log (updates against PRD §15)

Tick the row if the risk has *materialized* (not if mitigation succeeded).

- [ ] RK1 — CC1101 SPI sharing failed at runtime (note: ____________)
- [ ] RK2 — FIAT capture harder than expected (note: ____________)
- [ ] RK3 — EMV parse failed on Egyptian card scheme (note: ____________)
- [ ] RK4 — Deauth ineffective due to PMF (note: ____________)
- [ ] RK5 — Rogue AP unstable (note: ____________)
- [ ] RK6 — SD corruption (note: ____________)
- [ ] RK7 — Team member out (note: ____________)
- [ ] RK8 — Viva challenge unanswered (note: ____________)
- [ ] RK9 — RF noise floor swamps fixed-code car-key capture (note: ____________)
- [ ] RK10 — Modern car has no observable 433/868 OOK (smart-key / UWB) (note: ____________)
- [ ] RK11 — Mascot polish over-runs sensor work (note: ____________)
- [ ] RK12 — PN532 + OLED I²C contention under load (note: ____________)
- [ ] RK13 — Battery brownout under TX burst (note: ____________)
- [ ] RK14 — Charge circuit not certified for charge-while-load (note: ____________)

---

## Blockers (live)

Anything `- [!]` above is mirrored here with owner + date raised + what unblocks it.

| Date | Item | Owner | What unblocks it | Status |
|------|------|-------|------------------|--------|
|      |      |       |                  |        |

---

## Cut log

Anything `- [~]` above is mirrored here with the reason and which RK it ties to.

| Date | Item | Reason | Tied to RK |
|------|------|--------|-----------|
|      |      |        |           |

---

*Update this file at end of every day. Commit it with the daily diary entry.*
