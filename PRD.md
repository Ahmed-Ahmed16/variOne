# VariOne — Product Requirements Document

**Version:** 0.1 (MVP)
**Status:** Proposal for academic supervision review
**Owner:** VariOne project team, CIC New Cairo
**Faculty supervisor:** Dr. Ahmed Gaber, Networks Department
**Demo target:** Day 10 from project kickoff
**Document audience:** Project supervisors, faculty review board, internal engineering team

---

## 1. Executive Summary

VariOne is a handheld, ESP32-based wireless security awareness platform designed to make abstract digital-security risks **physically visible** in a controlled environment. The device combines four common attack surfaces into a single demonstrator — sub-GHz radio, NFC/RFID, infrared, and Wi-Fi — so that an audience can watch, in seconds, how an opportunistic attacker can interact with vehicles, contactless cards, consumer electronics, and home/office networks using sub-USD 100 of off-the-shelf hardware.

The MVP exists to satisfy two outcomes:

1. **Academic:** a complete, defensible graduation project that demonstrates competence across embedded systems, RF, networking, and applied cybersecurity.
2. **Public-good:** a teaching tool that materially raises security awareness in the Egyptian market by showing — not telling — non-technical audiences why basic defenses (rolling-code immobilizers, WPA3, contactless transaction limits, NFC sleeves) matter.

The device is a **single prototype**, **closed-source**, operated only inside formally sanctioned test environments under documented authorization. It is not, and will not be presented as, an offensive tool.

---

## 2. Background and Motivation

Egypt's consumer and SME wireless-security posture lags behind comparable markets in three measurable ways that ground VariOne's choice of features:

- **Wi-Fi:** WPA2-PSK remains the dominant home and SME encryption mode. WPA3 and Protected Management Frames (802.11w), which neutralize the 802.11 deauthentication primitive, are uncommon outside enterprise deployments. The deauthentication-and-rogue-AP flow is therefore not a theoretical curiosity locally; it is a present-tense risk.
- **Vehicles:** A long tail of older vehicles and aftermarket alarm/center-lock kits use **fixed-code** sub-GHz remotes (typically 433.92 MHz). Capture-and-replay against these is trivial with sub-USD 10 of hardware.
- **Contactless payments:** Domestic schemes (Meeza) and international schemes (Visa/Mastercard) operating in Egypt allow **PIN-less contactless transactions up to EGP 600**. Public understanding of what data is exposed when a card is read at a distance, and what relay attacks can do, is effectively zero among non-specialists.

The core insight driving the project: in a market where security education is undervalued, **a working demo of the threat changes minds in 30 seconds in ways that a presentation does not**. VariOne is an awareness instrument first and a research platform second.

### 2.1 Prior art and references

- **ESP32 Marauder** (`https://github.com/justcallmekoko/ESP32Marauder`) — open-source ESP32 Wi-Fi pentest firmware. We treat it as a behavioral reference for F4–F6: deauth frame structure, beacon spam pacing, evil-twin SoftAP setup, channel-hop strategy. VariOne does **not** vendor or fork Marauder source (license incompatibility — Marauder is GPL, VariOne is closed). Behaviors and constraints are reimplemented from scratch with attribution in the relevant module headers.
- **Samy Kamkar's RollJam / fixed-code rolling-code work** — pedagogical framing for why fixed-code RF is insecure (basis of S1 narrative).
- **Flipper Zero** — adjacent product class; informs the menu/UX expectations of a non-technical audience seeing the device for the first time, but no code or assets are reused.

---

## 3. Goals and Non-Goals

### 3.1 In-scope (MVP)

| # | Capability | Purpose |
|---|---|---|
| G1 | Sub-GHz capture and replay (300–928 MHz, focus 433.92 MHz) | Demonstrate fixed-code remote attack against the team's own 1988 FIAT 128 with custom-built center lock |
| G2 | NFC/RFID read at ISO/IEC 14443A | Demonstrate passive data leakage from contactless bank cards (PAN, expiry, name) and transit/access cards |
| G3 | IR receive and transmit | Demonstrate "universal remote" effect on TVs, ACs, projectors — high-impact, low-stakes audience hook |
| G4 | Wi-Fi 2.4 GHz scan (APs and stations) | Reconnaissance baseline |
| G5 | Wi-Fi targeted deauthentication | Demonstrate Layer 2 denial-of-service against WPA2 networks |
| G6 | Wi-Fi rogue AP with captive portal | Demonstrate credential-capture phase of an "evil twin" attack |
| G7 | SD card persistence of all captures | Permit later replay; permit data-decay/durability research |
| G8 | OLED-driven menu UI with 4-button navigation | Standalone, untethered operation during the demo |

### 3.2 Out-of-scope (this MVP)

- Rolling-code attacks (KeeLoq, Hitag2, RollJam) — not required for the team's chosen vehicle target.
- WPA3-SAE attacks, PMKID cracking, handshake offline attacks.
- EMV transaction replay or card cloning (technically blocked by per-transaction cryptograms; see §9.2).
- Bluetooth attacks.
- ~~Battery-powered operation~~ — **revised**: standalone battery operation is now an in-scope stretch goal for the final demo. See §6.4. The MVP still ships USB-C-capable; battery is additive, not a replacement.
- Open-source release. Source remains private to the team, faculty, and the partner organizations under the existing NDA.
- Mass-production hardware (one unit only).

### 3.3 Explicit research questions

- R1: What are the practical capture-to-replay success rates and signal decay characteristics for fixed-code sub-GHz remotes when stored on consumer-grade microSD over time?
- R2: How does reception range for ISO 14443A varies with PN532 antenna geometry and target card type?
- R3: Under single-radio concurrent operation (deauth + VariPortal on the same channel), what is the median time-to-association for a deauthed station to connect to the cloned SSID, and how does deauth duty cycle affect portal responsiveness and capture success rate?

---

## 4. Users and Scenarios

### 4.1 Primary users

- **The VariOne operator:** a member of the project team running the device during a demonstration or supervised lab session.

### 4.2 Audience (the people the demo is *for*)

- Faculty review board.
- Government and corporate stakeholders during sanctioned site visits.
- University students during awareness sessions delivered through CIC New Cairo.

### 4.3 Demo scenarios

- **S0 — "Cold open / wow factor":** VariOne boots into a short, high-confidence
  opener before the operator starts the technical flow: animated mascot,
  firmware/version identity, dual-core ESP32 status, and the four demo surfaces
  (`RF / NFC / Wi-Fi / IR`) shown as ready. This is presentation polish, but it is
  also a trust signal: the audience should understand in the first few seconds
  that this is a purpose-built awareness platform, not a loose breadboard demo.

- **S1 — "Your car is not locked":** Operator captures the unlock signal from the team-owned FIAT 128 once, then replays it from across the room. Door unlocks.
- **S2 — "Your wallet is shouting":** Operator reads the operator's own bank card, displays PAN and expiry on the OLED, and explains what an attacker on a crowded metro could collect at <5 cm range.
- **S3 — "Universal disrespect":** Operator captures the lab AC remote, then replays it. Audience laughs. Then the operator explains the same chip family is in IR-controlled industrial equipment.
- **S4 — "Your network is a polite host":** Operator selects a lab-controlled SSID. The VariPortal (cloned AP + captive portal) starts on the target channel, then deauthentication begins — the audience watches the lab phone drop off the real AP and automatically associate with the cloned SSID, where it is presented with a credential-capture portal themed to match the lab environment. This is the full, uninterrupted evil-twin attack chain running in real time, demonstrating the actual threat as it would occur in the wild. See §9.6.1 for the single-radio technical approach and §5 for the authorization framework.

---

## 5. Legal, Ethical, and Operational Framework

### 5.1 Authorization

The device is operated only inside test environments where written, organization-level authorization exists. Three such environments have been formally arranged for VariOne:

1. The CIC New Cairo Networks Department lab, under Dr. Ahmed Gaber's supervision.
2. A partner banking institution, for sanctioned NFC/contactless testing under signed NDA.
3. A national company, for sanctioned network-security testing under signed NDA.

Documentation of these authorizations is held by the college.

### 5.2 Egyptian legal context

The team operates in awareness of **Law 175 of 2018 (Anti-Cyber and Information Technology Crimes Law)**. All operation outside the three sanctioned environments above is prohibited, including casual demonstrations. The device firmware contains no functionality that targets named external networks, services, or persons.

### 5.3 Data handling

- All data captured during MVP testing is **synthetic test data** generated for the project, not live data of uninvolved third parties.
- The sole purpose of writing capture data to SD is research question R1 (durability/decay) and replay verification.
- Captured Wi-Fi rogue-portal credentials in S4 are entered by the operator on a test handset, are themselves test strings, and are zeroized from the SD card after each demo session.
- The device does not transmit any captured data off-board over the network.

### 5.4 Operational posture

All Wi-Fi, RF, NFC, and IR capabilities are **unrestricted within the three authorized environments** listed in §5.1. The device is a penetration-testing and awareness tool; artificial limitations that prevent the demonstration of real-world attack chains defeat the project's educational purpose. Operational rules reflect authorization scope, not firmware restrictions:

- **Wi-Fi deauthentication** may target any station or set of stations on the authorized test network, including broadcast deauth (`FF:FF:FF:FF:FF:FF`), single-station unicast, or all-discovered-station sweeps. The operator selects the mode; the firmware does not refuse any lawful targeting option within the authorized environment.
- **Deauthentication and the cloned AP (VariPortal) may run simultaneously** to demonstrate the full evil-twin attack chain in real time. See §9.6.1 for the single-radio technical approach and constraints.
- **Portal theming** may reproduce the branding, layout, and login flow of any authorized target (including the CIC university portal and partner organization portals covered by the signed NDAs). Realistic impersonation is the point of the test — a generic page does not measure the target's actual exposure. Non-authorized brands are not used.
- Sub-GHz transmission is bounded to power levels sufficient for the demo room.
- IR transmission targets only equipment present in the demo room.
- **Outside the three authorized environments, the device is not operated.** This is the safety boundary — not firmware-level locks on individual capabilities.

---

## 6. System Architecture

### 6.1 Architecture diagram (textual)

```
           ┌──────────────────────────────────────────────────┐
           │              ESP32-WROOM-32D N4                   │
           │         (DevKit V1, 38-pin, dual-core)            │
           │                                                   │
           │  ┌────────┐  ┌─────────┐  ┌───────┐  ┌─────────┐  │
           │  │ Wi-Fi  │  │ Menu /  │  │ FS /  │  │ Power / │  │
           │  │ stack  │  │ UI FSM  │  │ logger│  │ status  │  │
           │  └───┬────┘  └────┬────┘  └───┬───┘  └─────────┘  │
           │      │            │           │                   │
           │  ┌───┴────┐  ┌────┴────┐  ┌───┴───┐               │
           │  │ Deauth │  │ Input   │  │ NFC   │               │
           │  │ VariPo │  │ (4 btn) │  │ ctrl  │               │
           │  │ Portal │  └────┬────┘  └───┬───┘               │
           │  └────┬───┘       │           │                   │
           │       │     ┌─────┴────┐   ┌──┴────┐  ┌────────┐  │
           │       │     │ OLED I2C │   │ SubGHz│  │ IR ctrl│  │
           │       │     │ driver   │   │ ctrl  │  │        │  │
           │       │     └────┬─────┘   └──┬────┘  └───┬────┘  │
           │       │          │            │           │       │
           └───────┼──────────┼────────────┼───────────┼───────┘
                   │          │            │           │
              (built-in)  ┌───┴───┐    ┌───┴───┐   ┌───┴────┐
                          │ SH1106│    │CC1101 │   │ IR LED │
                          │ OLED  │    │ +ant. │   │ + TSOP │
                          └───────┘    └───────┘   └────────┘
                                                    
                          ┌──────┐    ┌────────┐
                          │PN532 │    │microSD │
                          │ NFC  │    │ card   │
                          └──────┘    └────────┘
                          
                            (CC1101 on VSPI, SD on dedicated
                             SPI bus GPIO 27/16/17/5,
                             OLED + PN532 on I2C, IR is
                             bit-banged GPIO)
```

### 6.2 Bus topology

- **I²C bus** (default ESP32 pins, SDA=21 / SCL=22): SH1106 OLED **and** PN532 NFC (PN532 in I²C mode at address 0x24). This is the verified working configuration in the v0.4 firmware. Earlier drafts of this PRD specified PN532 on SPI; that was reversed because the I²C bring-up succeeded first and is shipping.
- **VSPI bus** (CC1101 only): SCK=18, MISO=19, MOSI=23, CS=15. CC1101 is the sole device on VSPI.
- **SD SPI bus** (dedicated): SCK=27, MISO=16, MOSI=17, CS=5. The microSD card runs on its own SPI bus using GPIO 27/16/17, **not** on the shared VSPI. This eliminates the CC1101/SD bus contention that previously required mutex coordination for every SPI transaction.
- **GPIO direct**: IR TX LED, IR RX TSOP, 4 user buttons.

CC1101 is the sole device on the VSPI bus, so no SPI mutex is required for CC1101 transactions. The microSD card runs on a separate, dedicated SPI bus (GPIO 27/16/17/5), eliminating the bus contention that was a risk in earlier hardware revisions. PN532 on the OLED I²C bus adds one device at a non-conflicting address (0x24 vs OLED 0x3C); the bus has spare bandwidth at the 100 kHz default speed.

### 6.3 Voltage and current

- Power source: USB-C 5 V from bench supply for the entire MVP.
- ESP32 onboard LDO produces 3.3 V for all peripherals.
- CC1101 is **strictly 3.3 V** — VCC is fed from the ESP32 3.3 V rail, never from 5 V. CC1101 logic levels are 3.3 V and directly compatible with ESP32 GPIO; **no level shifter is required**.
- PN532 modules accept 3.3–5 V on VCC due to onboard regulation, with 3.3 V logic — directly compatible with ESP32.
- microSD modules with onboard regulator and level-shifters accept 5 V; bare microSD breakouts must be on 3.3 V.

Current budget (worst case, simultaneous, observed-conservative):
- ESP32 Wi-Fi TX: ~240 mA
- CC1101 TX: ~35 mA
- PN532 active: ~150 mA
- SD card write: ~100 mA
- OLED: ~20 mA
- IR TX (pulsed): ~50 mA average

Total worst-case ~600 mA — well within USB-C bus capacity. No additional regulation needed.

### 6.4 Battery (standalone operation, stretch)

**Constraints from the team:** small physical footprint (must fit inside the 3D-printed enclosure without making it pocket-hostile), long runtime per charge (one full demo day = ≥4 h at idle/scan duty cycle, ≥1 h with frequent TX), and **USB-C native charging** (single-cable workflow with the rest of the kit — no micro-USB adapter).

The form factor that hits all three is a **single Li-ion / LiPo cell + USB-C TP4056 (protected) + 5 V boost**. Architecture A is the recommended baseline; B and C are fallbacks.

**Architecture A — recommended: 18650 + USB-C TP4056-protected + MT3608 boost.**

```
USB-C in ─┐
          ↓
[USB-C TP4056 + DW01 protection]
          ↓ (cell terminal, 3.0–4.2 V)
   [18650 cell, 3000 mAh, protected]
          ↓
   [SPST hard-off switch]
          ↓
   [MT3608 boost to 5.0 V]
          ↓
       ESP32 VIN
          ↓ (onboard LDO)
       3.3 V rail → all peripherals
```

- **Cell:** Samsung 30Q / Sony VTC6 / Panasonic NCR18650B, 3000–3400 mAh, button-top **with built-in protection**. ~120–200 EGP from Sigma Electronics or Future Electronics in Cairo. Avoid noname / Ultrafire.
- **Runtime estimate:**
  - Idle (OLED on, scan polling): ~120 mA → ~25 h on 3000 mAh
  - Active scan (Wi-Fi promiscuous + OLED): ~250 mA → ~12 h
  - Worst case (Wi-Fi TX deauth burst + CC1101 TX + OLED + SD): ~600 mA peak, ~350 mA average → ~8 h
- **Charge module:** USB-C TP4056 with DW01-A + 8205A protection (search "TP4056 USB-C 1A protected"). ~30–45 EGP. The USB-C variant exposes a USB-C jack on the module — solder the cell + load pads, expose the jack through the enclosure.
- **Boost module:** MT3608 (≤2 A, 28 V). ~25 EGP. Trim the multiturn pot to **5.0 V exactly** before connecting the ESP32, then dab the pot with epoxy so it cannot drift.
- **Total Cairo BOM cost:** ~200–270 EGP.
- **Form factor:** 18650 is 18 × 65 mm — fits a side-pocket of the enclosure. Whole battery sub-assembly (cell + holder + TP4056 + boost) is roughly 70 × 25 × 20 mm if laid flat.

**Architecture B — slim LiPo 1500–2000 mAh + USB-C TP4056 + boost.**

- Thinner enclosure option: 103450 LiPo pouch (10 × 34 × 50 mm, ~1500–2000 mAh, ~3 h heavy / ~10 h idle).
- Same USB-C TP4056 + same MT3608 boost as A.
- Cell typically ~80–150 EGP. **Must** be a "protected" pouch (built-in PCM) or wired through the protected TP4056 board exclusively.
- Pick this only if Day-7 enclosure CAD cannot accommodate a cylindrical 18650.

**Architecture C — fallback: USB-C power bank.**

- Off-the-shelf USB-C power bank (5 V, ≥3000 mAh). Zero design risk. Ugly but works.
- Keep one in the demo kit regardless of which of A/B is chosen.

**Hard rules (apply to all architectures).**

- CC1101 is still **3.3 V only** on VCC. The 5 V boost feeds ESP32 VIN; ESP32's onboard LDO continues to produce the 3.3 V rail for every peripheral. Do not feed peripherals from the boost output.
- No bare cell without protection. The TP4056 module **must** be the protected variant (DW01 or equivalent on board), or the cell itself must be protected, or both.
- Charging-while-running is not formally validated on cheap TP4056 modules. Treat the device as **charge-or-run** until benched. Demo workflow: charge between scenarios, run from battery during them.
- **SPST slide switch** (1 A) between cell and boost input — hard-off for storage and transport (airline rules forbid armed Li-ion).
- **Low-battery monitor:** 100 kΩ / 100 kΩ divider from cell + into ADC1 pin **GPIO 35** (input-only, free). Firmware reads every 5 s, shows mascot `SAD` badge below ~3.6 V (~20% SOC), halts radios and shows full-screen `FAIL` below ~3.3 V (cutoff before TP4056 hard-cuts at 2.5–2.8 V).
- **Bulk capacitor on 5 V rail:** 1000 µF electrolytic + 100 nF ceramic across the boost output, mounted close to ESP32 VIN. Suppresses sag during Wi-Fi TX bursts (mitigation for RK13).
- **Thermal:** TP4056 runs warm at 1 A. Leave ≥5 mm clearance and one ventilation slot above the charge module.

**Decision rule.** Pick A by default. Switch to B only if Day 7 enclosure CAD shows the 18650 will not fit. Architecture C ships in the kit either way as fallback.

---

## 7. Hardware Bill of Materials and Pin Map

### 7.1 BOM

| Qty | Part | Notes |
|-----|------|-------|
| 1 | ESP32-WROOM-32D N4 DevKit V1 (38-pin) | Main MCU, dual-core CPU, single 2.4 GHz Wi-Fi radio |
| 1 | CC1101 433 MHz module (E07-M1101D V2.0) with whip antenna | Already on hand and wired |
| 1 | CC1101 spare module | On hand; reserved for future TX/RX split or A/B comparison testing |
| 1 | 433.92 MHz SAW bandpass filter (e.g. **B3551** or **B3555**) | Reject sensor / mains / ISM noise outside the car-key + garage-gate band — see §7.4 |
| 1 | RF LNA module (e.g. SPF5189Z evaluation board) | Optional gain stage in front of CC1101 RX for weak fobs at distance — see §7.4 |
| 1 | PN532 NFC module (red/blue PCB, V3) | Configured for **I²C mode** (SEL0=1, SEL1=0). Address 0x24. Shares the OLED I²C bus. |
| 1 | microSD card module (3.3 V or 5 V regulated) | FAT32 |
| 1 | microSD card, 8–32 GB, Class 10 | FAT32 formatted |
| 1 | SH1106 128×64 OLED, I2C | U8g2-compatible |
| 1 | IR LED (940 nm) + NPN driver transistor + 100 Ω resistor | Bit-banged 38 kHz carrier |
| 1 | TSOP38238 (or equivalent 38 kHz IR receiver) | Demodulated IR input |
| 4 | Tactile push buttons (momentary) | UP / DOWN / SELECT / BACK |
| 4 | 10 kΩ resistors | Optional external pull-ups for button pins |
| 1 | 3D-printed enclosure | Designed in-house |
| — | Hookup wire, headers, perfboard or custom PCB | — |
| 1 | 18650 Li-ion cell (3000–3400 mAh, **protected**, button-top) **or** LiPo pouch 1500–2000 mAh (103450) | Battery cell — see §6.4. 18650 preferred for runtime. |
| 1 | **USB-C TP4056** charge module **with DW01-A + 8205A protection** | Charge + protect cell from USB-C — must be USB-C variant, must be protected |
| 1 | MT3608 boost converter to 5 V | Step cell up to ESP32 VIN; trim pot to 5.0 V then epoxy |
| 1 | 1000 µF electrolytic + 100 nF ceramic | Bulk decoupling on 5 V rail at ESP32 VIN to suppress TX-burst sag (RK13) |
| 1 | SPST slide switch, 1 A rated | Battery hard-off (storage / airline) |
| 2 | 100 kΩ resistors (1% if available) | ADC divider on GPIO 35 for low-battery cutoff |
| 1 | 18650 cell holder (single bay, soldered tabs) | Mechanical retention for architecture A |

### 7.2 Pin map (ESP32 DevKit V1)

This pin map is **the single source of truth** for the firmware and the wiring harness. Any change requires updating both.

This pin map reflects the **as-built v0.4 working configuration**. Earlier draft assignments (PN532 on SPI, buttons on 32/33/13/14, IR TX/RX on 17/16, CC1101 CS=5 or 27 / GDO0=27) were superseded once the hardware came up; the table below is now authoritative.

| ESP32 GPIO | Function | Peripheral | Notes |
|---|---|---|---|
| GPIO 21 | I²C SDA | OLED + PN532 | Shared I²C bus, OLED 0x3C, PN532 0x24 |
| GPIO 22 | I²C SCL | OLED + PN532 | Shared I²C bus |
| GPIO 13 | PN532 IRQ | PN532 | Wire **physically connected**, currently unused by firmware (`Adafruit_PN532(255, 255)`). Reserved for future interrupt-driven card-present detection. |
| GPIO 18 | VSPI SCK | CC1101 | CC1101 SPI clock (CC1101 is sole VSPI device) |
| GPIO 19 | VSPI MISO | CC1101 | CC1101 SPI data in |
| GPIO 23 | VSPI MOSI | CC1101 | CC1101 SPI data out |
| GPIO 15 | CC1101 CSN | CC1101 | Current working wiring; must be set HIGH at boot before SPI init |
| GPIO 4 | CC1101 GDO0 | CC1101 | RX/TX state, RSSI gate, edge timing |
| — | CC1101 GDO2 | CC1101 | Leave unconnected (per Ebyte E07-M1101D V2.0 wiring) |
| GPIO 5 | SD CS | microSD | Dedicated SD SPI bus (not on VSPI) |
| GPIO 25 | IR TX | IR LED driver | 38 kHz LEDC carrier, NPN driver (planned — LED not yet purchased) |
| GPIO 36 | IR RX | VS1838B | Input-only pin, fine for demodulated IR input |
| GPIO 27 | SD SCK | microSD | Dedicated SD SPI clock (not on VSPI) |
| GPIO 14 | Button LEFT (BACK) | tactile sw | INPUT_PULLUP |
| GPIO 26 | Button UP | tactile sw | INPUT_PULLUP |
| GPIO 32 | Button RIGHT (SELECT) | tactile sw | INPUT_PULLUP |
| GPIO 33 | Button DOWN | tactile sw | INPUT_PULLUP |
| GPIO 2 | Onboard LED | status indicator | Already on DevKit |
| GPIO 0 | — | (boot mode) | Leave alone |
| GPIO 6–11 | — | (flash, reserved) | DO NOT USE |
| GPIO 12 | — | (strapping, reserved) | Leave floating unless the full pin map is revised |
| GPIO 1, 3 | UART0 | USB serial debug | Leave for serial console |
| GPIO 16 | SD MISO | microSD | Dedicated SD SPI data in |
| GPIO 17 | SD MOSI | microSD | Dedicated SD SPI data out |

### 7.3 Wiring notes

- CC1101 CS (GPIO 15) must be initialized HIGH at boot before any SPI activity. The firmware sets the CS pin HIGH as the first action in the CC1101 driver's `begin()`. Specifically, `digitalWrite(PIN_CC1101_CSN, HIGH)` must run **before** any SPI activity to prevent the bus conflict that previously crashed the OLED.
- The microSD card runs on a **dedicated SPI bus** (SCK=27, MISO=16, MOSI=17, CS=5), separate from the CC1101's VSPI bus. This eliminates the SPI bus contention between CC1101 and SD that was a problem in earlier hardware revisions. The SD card driver initializes its own `SPIClass` instance on the dedicated pins — it does not share the default VSPI `SPI` object used by CC1101.
- The PN532 module is operated in **I²C mode**: SEL0=1, SEL1=0 (verify against silkscreen). Address 0x24, SDA=21, SCL=22, **IRQ physically wired to GPIO 13** (firmware currently polls and ignores the IRQ — `Adafruit_PN532(255, 255)` — but the wire is there for future interrupt-driven detection without rework).
- The CC1101 module on hand is the Ebyte E07-M1101D V2.0 (433 MHz). Antenna already attached. GDO2 is left unconnected.
- The IR LED is driven through an NPN (e.g. 2N3904 or BC547) with the LED in the collector path and a current-limiting resistor (100 Ω for ~30 mA continuous, lower for pulsed). The ESP32 GPIO drives the base through a 1 kΩ resistor.

### 7.4 RF front-end: amplifier and filter for car-key / garage-gate focus

Sub-GHz capture in an urban Egyptian environment is dominated by noise from 433 MHz sensors (weather stations, doorbells, mains-powered remote outlets, building-management telemetry) plus broadband noise from switching power supplies. To make F1 (§9.1) reliably target **car key fobs** and **garage-gate / barrier remotes** without the user having to filter manually, the radio path is:

```
antenna → [ optional LNA ] → [ 433.92 MHz SAW bandpass ] → CC1101 RX
                                                          ↓
                                                CC1101 TX → antenna (TX bypasses filter chain)
```

**SAW bandpass** narrows the front end to roughly 433.92 ± 1 MHz, rejecting sensors that drift outside the slot and rejecting all 315 / 868 / 915 MHz traffic. **LNA** (optional) recovers ~15–20 dB of gain lost to filter insertion + cable, useful only if real-world capture distance proves marginal in §13/§9.1 testing — install it only after a non-LNA capture attempt has been benchmarked, otherwise the LNA will simply amplify nearby noise.

**Egypt-sourcing options (price-ranked, cheapest first):**

1. **Spare CC1101 + software-only filtering** (free — already on hand). Use a tighter `RX bandwidth` register setting (e.g. 58 kHz instead of the default 200 kHz) plus a stricter RSSI gate. No new hardware; only register tuning. Try this first.
2. **AliExpress 433 MHz SAW bandpass module** — search "433 SAW filter B3551" or "B3555". Typical 30–80 EGP, lead time 2–3 weeks. Soldered inline between antenna and CC1101 RF input.
3. **SPF5189Z LNA evaluation board** (AliExpress / RAMSEY / Sigma Electronics Cairo) — typical 200–400 EGP. ~20 dB gain, 50–4000 MHz, runs on 3.3 V. Pair with the SAW filter for a proper RX chain.
4. **Pre-built 433 MHz "LoRa" RF front-end module** (e.g. RFM69 + SAW). 250–500 EGP from Future Electronics or Ram Electronics in Cairo. Only if a discrete LNA + filter prove unreliable.
5. **Custom matched filter on a copper-clad protoboard** — if budget is tight and we have access to the lab vector network analyzer, design a simple LC bandpass + RF amp with a BFR93 transistor. Cost ~50 EGP plus labor; risk: tuning takes a full afternoon.

**Decision rule.** Start with option (1). If §9.1 AC1 fails on a captured car-key fob in an urban-noise environment, install option (2). Only escalate to option (3) or beyond if RX sensitivity, not interference rejection, is the demonstrated bottleneck. Document the decision in `docs/rf-frontend.md` with the measurement that drove it.

**Power budget.** SPF5189Z draws ~70–90 mA; SAW filter is passive. If the LNA is installed permanently it must be gated by a GPIO so it does not run during TX.

---

## 8. Software Architecture

### 8.1 Toolchain

- **IDE / build system:** PlatformIO on VS Code.
- **Framework:** Arduino-ESP32 (`framework = arduino`) on `platform = espressif32`.
- **Board:** `board = esp32dev`.
- **Firmware partitioning:** default Arduino partition with SPIFFS (we'll keep the captive portal HTML/CSS in flash via SPIFFS or as PROGMEM strings — see §8.4).

`platformio.ini` (target shape):

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600
board_build.partitions = default.csv
build_flags =
    -DCORE_DEBUG_LEVEL=3
lib_deps =
    olikraus/U8g2
    SPI
    Wire
    SD
    elechouse/PN532@^1.3.2
    SmartRC-CC1101-Driver-Lib
    crankyoldgit/IRremoteESP8266
    ; ArduinoJson for capture file metadata
    bblanchon/ArduinoJson@^7.0.0
```

### 8.2 Module breakdown

The codebase is organized around one C++ class per peripheral and one orchestrator. CC1101 is the sole device on the VSPI bus; the microSD card runs on a dedicated SPI bus (GPIO 27/16/17/5) and does not share VSPI. No cross-peripheral SPI mutex is required.

```
src/
├── main.cpp                  # setup(), loop(), top-level state
├── config.h                  # PIN_*, FREQ_*, version, build flags
├── core/
│   ├── SpiBus.{h,cpp}        # SPI helpers; CC1101 on VSPI, SD on dedicated bus (GPIO 27/16/17/5)
│   ├── EventLoop.{h,cpp}     # cooperative scheduler
│   ├── Logger.{h,cpp}        # serial + SD log
│   └── StatusLed.{h,cpp}     # GPIO 2 patterns
├── ui/
│   ├── Display.{h,cpp}       # U8g2 wrapper
│   ├── Input.{h,cpp}         # 4-button debounce, long-press
│   ├── MenuTree.{h,cpp}      # static menu definition
│   └── Screens.{h,cpp}       # one screen per page (Marauder-style)
├── storage/
│   ├── SdCard.{h,cpp}        # mount, FAT helpers
│   └── CaptureStore.{h,cpp}  # write/read NFC, sub-GHz, IR captures
├── radio/
│   ├── SubGhz.{h,cpp}        # CC1101 init, scan, capture, replay
│   └── SubGhzCodec.{h,cpp}   # OOK/2-FSK helpers, fixed-code framing
├── nfc/
│   ├── NfcReader.{h,cpp}     # PN532 wrapper, ISO 14443A
│   └── EmvLite.{h,cpp}       # parse PPSE/AID, PAN, expiry, name
├── ir/
│   ├── IrRx.{h,cpp}          # raw + decoded receive
│   └── IrTx.{h,cpp}          # raw + protocol transmit
└── wifi/
    ├── WifiScan.{h,cpp}      # AP + station scan in monitor mode
    ├── Deauth.{h,cpp}        # crafted 802.11 deauth tx (all targeting modes)
    ├── VariPortal.{h,cpp}    # SoftAP with cloned SSID + concurrent deauth coordination
    └── Portal.{h,cpp}        # captive portal HTTP server + DNS + theme loader
```

### 8.3 Cooperative scheduling

The loop is a fixed-tick (1 ms) cooperative scheduler. Long-running operations (deauth burst, sub-GHz capture window) are non-blocking and yield to the menu input handler so the user can always cancel with BACK.

### 8.4 Captive portal assets

Generic portal themes for S4 are embedded in firmware (PROGMEM strings) as defaults. **Target-specific themes** (authorized portal reproductions for CIC, partner bank, and partner company) are loaded from the SD card at `/portal-themes/<theme-name>/` — each folder contains `index.html`, `style.css`, and optionally image assets, plus a `theme.json` manifest with the theme display name and description. This SD-based approach means target themes can be updated without reflashing firmware, and sensitive branding assets are not compiled into the binary. Portal submissions are written to `/captures/portal.log` with timestamps. The log file is retained for post-demo review and audit.

---

## 9. Feature Specifications

For each feature: purpose, technical approach, acceptance criteria, and (where the team's expectations need correction) a **reality check** the project will own honestly.

### 9.1 F1 — Sub-GHz capture and replay

**Purpose.** Demonstrate fixed-code remote attacks on a team-owned 1988 FIAT 128 with custom-built center lock, **and** demonstrate by negative result why modern rolling-code vehicles defeat the same attack. Secondary target: lab-owned **garage-gate / barrier remotes** (typical apartment-block and parking-garage fobs in Cairo are still fixed-code PT2262 / EV1527 at 433.92 MHz).

**Technical approach.** CC1101 configured in OOK/ASK mode at 433.92 MHz, deviation/bandwidth/data-rate matched to typical aftermarket fixed-code remotes (~2–4 kbps). RX bandwidth register tightened to 58 kHz to suppress sensor / mains noise (see §7.4). Capture path: enter monitor state, threshold on RSSI, sample GDO0 timing into a circular buffer, end-detect on inter-edge timeout, persist to SD as JSON with `{freq, modulation, rssi_dbm, edges_us[], timestamp, target_class}`. Replay path: load capture, retransmit edges via GDO0/TX FIFO with the same modulation parameters.

`target_class` field values: `fixed_code_car`, `fixed_code_gate`, `rolling_code_car_attempt`, `unknown`. Used by the UI to set the right narrative + mascot mood on replay.

**UX.** Menu: `Sub-GHz → Read RAW` (live capture, abort with BACK), `Sub-GHz → Saved → <file> → Replay`. Mascot is `WORKING` while listening, `HAPPY` on a clean capture, `SAD` on capture timeout.

**Acceptance criteria.**
- AC1: Replaying a captured FIAT 128 unlock signal at ≤5 m line-of-sight unlocks the doors in ≥4 of 5 attempts.
- AC2: The OLED displays the detected center frequency and a peak RSSI during capture.
- AC3: A capture file written to SD is reloadable across reboots and replays successfully.
- AC4: Replaying a captured **fixed-code garage-gate** signal opens the gate in ≥3 of 5 attempts at ≤10 m.
- AC5: For each modern vehicle in §9.1.1, the team captures a key-fob press, attempts replay at ≤2 m, and **records the failure** (no door unlock) along with a one-line explanation of why (rolling code / KeeLoq / Hitag2 / proprietary).

**9.1.1 Modern-vehicle comparison set (negative-result demos).** The team has access to the following owned vehicles for sanctioned comparison testing only. Each is captured and replayed once; failure is the expected and pedagogically useful result:

| Vehicle | Year | Expected fob scheme | Expected outcome |
|---|---|---|---|
| FIAT 128 (custom center lock) | 1988 | Fixed-code OOK 433.92 MHz | **Unlock** (positive demo, S1) |
| Mini Cooper (Dr. Gaber) | 2016 | Rolling-code (KeeLoq or proprietary BMW) | Replay rejected |
| JAC S3 | 2018 | Rolling-code (likely KeeLoq variant) | Replay rejected |
| Avatr (team-owned) | recent | Smart-key / UWB / rolling | Replay rejected (likely no OOK at all on 433) |
| Mercedes E180 | recent | Hitag2 / proprietary 433 or 868 | Replay rejected |

For each vehicle the team logs: detected frequency band, observed modulation (OOK / FSK / FHSS), RSSI, number of edges in burst, and whether the burst differs across two consecutive presses (a sign of rolling code). This data set is the **strongest single artifact** for the §9.1 reality-check narrative — "we tried, here is the data, this is why modern fobs are safer."

**Reality check.** The FIAT 128 unlock works specifically because the lock is fixed-code. Capture-and-replay against any vehicle with a rolling-code immobilizer (essentially every OEM key fob from ~1996 onward) will not work, and we will not pretend otherwise. The §9.1.1 comparison set is the demo narrative for that point — instead of asserting it, we *show* it.

### 9.2 F2 — NFC read of contactless bank cards

**Purpose.** Demonstrate the data exposure of contactless EMV cards and the realistic risk surface around the EGP 600 PIN-less limit.

**Technical approach.** PN532 over **I²C** (shared with OLED, address 0x24) in ISO/IEC 14443A read mode. IRQ wire is connected on GPIO 13 but firmware currently polls and does not subscribe to the interrupt (constructor `Adafruit_PN532(255, 255)`); switching to IRQ-driven detection later is a one-line change since the wire is in place. On card detect: read UID/SAK/ATQA; if EMV, send `SELECT PPSE` (`2PAY.SYS.DDF01`), parse the FCI to get the AID, `SELECT AID`, `GET PROCESSING OPTIONS` and `READ RECORD` to extract Track 2 equivalent data — primary account number (PAN), expiry, cardholder name where present (often absent on modern issuance). All parsed fields render to OLED and write to `/captures/nfc/<timestamp>.json`. The current v0.4 firmware already detects card presence at 0x24 and reads UID + EMV AID; PAN/expiry parse + masking is the §9.2 work that remains.

**UX.** `NFC → Read Card` shows live UID; `NFC → Read Bank Card` runs the EMV-lite parse and displays masked PAN + expiry.

**Acceptance criteria.**
- AC1: Reads a contactless bank card at ≥2 cm reliably and displays masked PAN (`**** **** **** 1234`) and expiry within 2 seconds.
- AC2: On non-EMV cards (transit, building access), falls back to UID-only display.
- AC3: A captured card file is replayable as a UID-emulation only — see Reality Check.

**Reality check — important and the demo narrative depends on getting this right.** The original team framing was "replicate the signal and spend EGP 600, then EGP 600 again." **This is not how contactless EMV works.** Every contactless transaction includes a dynamic Application Cryptogram (AC) generated by the card's secure element from the issuer's master key, the application transaction counter (ATC), and unpredictable terminal data. A captured signal cannot be replayed at a different terminal — the issuer will reject it. A passive read also does not yield the master key.

What VariOne **can honestly demonstrate**, and what is genuinely the awareness message:

1. **Passive data leakage at distance** — PAN, expiry, and (where present) name can be read by a hostile reader through a wallet/pocket at a few centimeters. With an upgraded antenna, this range grows. For a non-technical audience this is shocking.
2. **The contactless limit normalizes attack volume** — relay attacks (Mole-style: a hostile reader near the victim, a hostile card emulator near a real terminal, both bridged over IP) can perform a *real* transaction up to EGP 600 without the card leaving the victim's pocket. VariOne does not implement the relay leg in MVP, but the OLED narrative for the demo explains the threat model honestly using the data we *did* extract.
3. **Reusability/decay research (R1)** — captured data is stored on SD for the project's durability study, not for unauthorized reuse.

We will **not** claim, on stage or in the report, that the device performs unauthorized payments. The team will be challenged on this in the viva and we want a clean answer.

### 9.3 F3 — IR receive and transmit

**Purpose.** High-engagement, low-stakes demo of how trivial consumer IR is. Doubles as a universal remote.

**Technical approach.** Receive: VS1838B / TSOP38238-compatible receiver → GPIO 36, captured as raw timing array using the IRremoteESP8266 library; common protocols (NEC, Sony SIRC, RC5/RC6, Samsung) are also auto-decoded. Transmit: 38 kHz carrier on GPIO 25 via LEDC PWM gated by data timing.

**UX.** `IR → Receive` (point a remote at the device, capture appears on screen with detected protocol), `IR → Saved → <file> → Transmit`.

**Acceptance criteria.**
- AC1: Capture and replay a TV power command from at least three different remote brands.
- AC2: Replay range ≥3 m line-of-sight to a typical TV.

### 9.4 F4 — Wi-Fi scan

**Purpose.** Reconnaissance baseline; populates target lists for F5 and F6.

**Current implementation status.** The v0.4 Wi-Fi implementation is not
accepted as feature-complete. AP scan, station discovery, target binding,
deauth, cloned AP (VariPortal), and captive portal are being rebuilt as the active Wi-Fi
revamp track. Existing code may be reused only where it still satisfies the
PRD specifications and passes hardware tests.

**Technical approach.** ESP32 Wi-Fi in promiscuous/monitor mode. AP scan via `WiFi.scanNetworks()` for the simple list; station scan via promiscuous-mode packet sniff filtering on data and probe frames to enumerate `(BSSID, station MAC, channel, RSSI)` tuples.

**UX.** `Wi-Fi → APs` (paginated list sorted by RSSI), `Wi-Fi → Stations` (live count, BACK to freeze and select).

**Acceptance criteria.**
- AC1: Lists ≥10 APs in the lab within 5 seconds.
- AC2: Allows the operator to mark a target AP and a target station for use by F5.

### 9.5 F5 — Deauthentication

**Purpose.** Demonstrate the 802.11 deauth primitive against WPA2 networks and serve as the first stage of the full VariPortal attack chain (F5 → F6).

**Current implementation status.** This feature is under active revamp. The
firmware must support all three targeting modes (single station, all-discovered sweep, broadcast) and integrate cleanly with the VariPortal (F6) for simultaneous operation.

**Technical approach.** Construct an 802.11 deauthentication management frame (reason code 7 — class 3 frame from non-associated STA) with the selected target as receiver and the target AP as transmitter, then inject via `esp_wifi_80211_tx()` on the target AP's channel. Bursts are time-bounded (default 15 s, configurable up to 60 s).

**Targeting modes** (operator-selectable from the F4 scan results):

- **Single station** — one selected client MAC. Useful for surgical demos where only a specific lab device is deauthed while others stay connected.
- **All discovered clients** — loops over station MACs found under the selected BSSID and sends individual unicast deauth frames to each. Useful when the demo needs to show all lab devices dropping off.
- **Broadcast** — sends deauth with destination `FF:FF:FF:FF:FF:FF` on the target AP's BSSID. This is the standard pentest technique: a single frame deauths every client on the AP. More efficient than the per-station sweep, but affects all devices in range of that AP on that channel. Appropriate only in the controlled lab environment where no uninvolved devices are present.

The operator selects the mode on the deauth config screen before starting the burst.

**UX.** `Wi-Fi → Deauth → Pick AP → Scan Stations → Pick Station / All Discovered / Broadcast → Run` shows a countdown bar, target info, and frame count. Mascot is `ANGRY` during the burst.

**Acceptance criteria.**
- AC1: A lab-owned phone associated with the lab AP loses connectivity within 5 seconds of starting the burst in any targeting mode.
- AC2: Deauth halts at the configured timeout and the radio returns to scan-ready state (or to VariPortal co-operation state if F6 is active).
- AC3: Broadcast mode sends `FF:FF:FF:FF:FF:FF` deauth frames and disconnects all clients on the target AP.
- AC4: All three targeting modes are selectable from the UI and function independently.
- AC5: Deauth operates concurrently with the VariPortal (F6) when both are active — see §9.6.1 for the single-radio integration approach.

**Reality check.** This is ineffective against WPA3-SAE and against WPA2 networks with PMF enabled (most modern enterprise APs and many recent home routers since ~2020). We test against an intentionally-WPA2 lab AP and we say so.

### 9.6 F6 — VariPortal (cloned AP with credential-capture portal)

**Purpose.** Demonstrate the complete evil-twin attack chain — cloned SSID + captive portal + credential capture — in a controlled environment, showing the authorized target audience how their users would experience a real attack against their infrastructure.

**Current implementation status.** This feature is under active revamp. The
portal must support realistic theming (including authorized target branding), log all submissions, and operate concurrently with F5 deauth for the full attack chain demo.

**Technical approach.** ESP32 in SoftAP mode broadcasts the cloned SSID (selected from F4 scan) as **open** (no PSK), pinned to the target AP's channel. A captive portal is presented via a DNS server that resolves all queries to the ESP32's IP, plus an HTTP server serving the portal page. Submitted credentials are displayed on the OLED (masked or unmasked at operator discretion) and written to `/captures/portal.log` with a timestamp for audit.

**Portal theming.** The operator can load and switch between portal themes designed to match the authorized testing targets:

- **Generic themes** (bundled defaults): `Network Access`, `Campus Lab Access`, `Router Console`. Used for general demos and university lectures.
- **Target-specific themes** (loaded from SD card): accurate reproductions of the authorized target's real login page — including the CIC university portal, partner banking institution portal, and partner national company portal as covered under the signed NDAs. The point of target-specific theming is to measure whether users in the target organization would be fooled by a real attack; a generic page does not answer that question.
- Theme assets (HTML, CSS, images) are stored on the SD card under `/portal-themes/<theme-name>/` and served from there. The firmware reads a `theme.json` manifest per theme folder.
- **Non-authorized brands are not reproduced.** Only portals belonging to organizations where the team holds written testing authorization (§5.1) are cloned. This is an operational discipline, not a firmware lock.

**UX.** `Wi-Fi → VariPortal → Pick SSID → Pick Theme → Start`. A live-status screen shows: cloned SSID name, channel, connected client count, association events, and portal submissions. If F5 (deauth) is running concurrently, the status screen shows both the deauth frame count and the VariPortal association count on a split display.

**Acceptance criteria.**
- AC1: A lab phone connecting to the cloned SSID is redirected to the captive portal within 10 seconds.
- AC2: Credentials submitted on the portal are written to SD and visible on the OLED.
- AC3: Stopping the VariPortal returns Wi-Fi to scan-ready within 3 seconds.
- AC4: Switching portal theme changes the captive portal presentation to match the selected authorized target.
- AC5: The VariPortal operates concurrently with F5 deauth on the same channel — a deauthed lab phone auto-associates with the cloned SSID and reaches the portal without operator intervention.
- AC6: Target-specific themes loaded from SD render correctly (logo, color scheme, form layout match the real portal).

**Demo sequencing.** In the full S4 demo, the operator starts the VariPortal first (cloned SSID broadcasting), then starts F5 deauth against the real AP. The audience watches the lab phone disconnect from the real network and connect to the clone in real time, then sees credentials appear on the OLED. This is the actual attack chain as it occurs in the wild — the educational impact depends on showing it uninterrupted. See §9.6.1 for the technical approach that makes this work on a single-radio ESP32.

### 9.6.1 Single-radio concurrent operation (deauth + VariPortal)

The ESP32-WROOM-32D has a **single 2.4 GHz radio**, which creates a technical challenge for running SoftAP (VariPortal) and raw frame injection (deauth) simultaneously. This is solvable — ESP32 Marauder demonstrates this on the same hardware — but requires understanding the constraints.

**How it works.** The ESP32 Wi-Fi stack allows SoftAP mode and raw frame injection (`esp_wifi_80211_tx()`) to coexist. The SoftAP runs on a fixed channel, and raw deauth frames can be injected on *any* channel by briefly switching the radio. The key constraint is:

- **Same-channel operation is reliable.** If the target AP, the VariPortal SoftAP, and the deauth injection all operate on the **same Wi-Fi channel**, there is no channel-switching overhead. The SoftAP serves the portal while deauth frames are injected between beacon intervals. This is the preferred configuration.
- **Cross-channel operation is degraded.** If the target AP is on a different channel from the SoftAP, the radio must hop between channels — injecting deauth frames on the target's channel, then returning to the SoftAP's channel to service portal clients. This causes latency spikes on the portal, dropped association attempts, and reduced deauth throughput. It works, but the demo is less clean.

**Lab setup recommendation.** For the S4 demo, configure the lab-controlled AP on a **known, fixed channel** (e.g. channel 6) and start the VariPortal SoftAP on the same channel. This eliminates channel-hopping entirely and gives the smoothest demo experience. Document the channel in the demo kit checklist.

**Implementation approach** (reference: ESP32 Marauder's `WiFiScan.cpp` attack loop):

1. Start SoftAP on the target channel with the cloned SSID.
2. Start the DNS + HTTP captive portal server on Core 0.
3. On Core 1 (or interleaved on Core 0 between portal servicing), inject deauth frames via `esp_wifi_80211_tx()` in timed bursts.
4. If cross-channel mode is required: implement a **time-division multiplexing** scheme — spend N ms on the target channel injecting deauth, then return to the SoftAP channel for M ms to service portal traffic. Typical values from Marauder: ~80% deauth duty, ~20% portal duty, with 100–200 ms switching intervals.
5. Monitor SoftAP association events; when a victim device associates, briefly pause deauth to let the portal handshake complete, then resume.

**Dual-core allocation** (ESP32-WROOM-32D has two cores):

- **Core 0:** Wi-Fi stack, SoftAP, TCP/IP, DNS server, HTTP portal server, association event handler.
- **Core 1:** Deauth frame injection loop, station scan refresh, OLED UI update, button input handler.

This split keeps the portal responsive while deauth runs at full rate. FreeRTOS task pinning (`xTaskCreatePinnedToCore`) is used to enforce the split.

**Known limitations to document in the demo:**

- Portal response time may increase by 100–300 ms during active deauth bursts, even in same-channel mode.
- If the lab AP is on a DFS channel (channels 52–144), the ESP32 cannot legally transmit there in most regions; pin the lab AP to channels 1–11.
- Maximum simultaneous VariPortal clients is limited by ESP32 SoftAP capacity (~4–8 stations depending on firmware configuration).
- The deauth-to-association race depends on the victim device's reconnection behavior; some devices take 2–10 seconds to probe for the cloned SSID after deauth.

### 9.7 F7 — SD card persistence

**Purpose.** Persist captures across reboots; enable research question R1 (decay).

**Technical approach.** FAT32 SD card mounted via the Arduino `SD` library on the dedicated SPI bus (SCK=27, MISO=16, MOSI=17, CS=5). Directory layout:

```
/captures/
   /subghz/<UTC-timestamp>_<freq>kHz.json
   /nfc/<UTC-timestamp>_<uid>.json
   /ir/<UTC-timestamp>_<protocol>.json
   /portal.log              (wiped end of session)
/system.log              (rolling, capped at 1 MB)
/config.json             (device config: brightness, default freq, deauth timeout, etc.)
```

Each capture file embeds a schema version and a SHA-1 of the payload for the decay study (R1: re-read SHA-1 vs. original at intervals).

### 9.8 F8 — Menu UI

Static menu tree, 4-button navigation (UP/DOWN scroll, SELECT enter, BACK exit), 128×64 OLED. Marauder-inspired layout but original design — no copied assets. Title bar shows current screen + one of: `WiFi`, `RF`, `NFC`, `IR`, `SD` mode tags. Status bar shows SD mount state and free RAM.

---

## 10. UX and Display

### 10.1 Screen taxonomy

- **Splash:** logo + firmware version + mascot boot animation, 1.5 s.
- **Main menu:** 5 entries — WiFi, Sub-GHz, NFC, IR, SD, Settings.
- **List screens:** scrollable, 5 visible rows on 128×64.
- **Action screens:** title + primary metric (RSSI bar, countdown, etc.) + transient log line at bottom + mascot in the bottom-right corner reflecting current mood.
- **Confirmation screens:** any destructive or attack-class action requires SELECT-hold (1 s) to confirm. Mascot turns `ANGRY` on the confirm screen for attack-class actions to reinforce intent.

### 10.2 Status indicators

- Onboard LED (GPIO 2): **slow blink** = idle, **fast blink** = capturing/scanning, **solid** = transmitting (any radio).
- OLED top-right corner: SD ✓/✗ + active radio glyph.

### 10.3 Mascot system (gamification layer)

The mascot is a first-class UX element retained from v0.4 and extended. Every screen and every interaction binds a mood. Goal: a non-technical viewer reads the mascot, not the text, to know what is happening.

**Available moods** (preserve from v0.4 — do not rename without updating all call sites): `IDLE`, `HAPPY`, `THINKING`, `SAD`, `ANGRY`, `SLEEPING`, `SUCCESS`, `FAIL`, `WORKING`, `WAVING`.

**Mood binding per screen / event:**

| Screen / event | Mood |
|---|---|
| Boot splash | `WAVING` (boot animation) |
| Idle main menu, no input >30 s | `SLEEPING` |
| Any sub-menu entry | `THINKING` |
| WiFi scan running | `WAVING` |
| WiFi scan: ≥1 AP found | `HAPPY` |
| WiFi scan: 0 APs | `SAD` |
| Packet monitor / probe sniffer running | `WORKING` |
| Probe / station found | `HAPPY` |
| Deauth attack start | `ANGRY` |
| Beacon spam running | `HAPPY` |
| VariPortal AP up | `WORKING` |
| VariPortal credential captured | `SUCCESS` |
| NFC scan running | `THINKING` |
| NFC card read (UID only) | `HAPPY` |
| NFC card read (EMV PAN) | `SUCCESS` |
| NFC scan timeout / no card | `SAD` |
| Sub-GHz capture listening | `WORKING` |
| Sub-GHz capture got edges | `HAPPY` |
| Sub-GHz capture timeout | `SAD` |
| Sub-GHz replay TX | `ANGRY` |
| Sub-GHz replay → target accepted (door / gate opens) | `SUCCESS` |
| Sub-GHz replay → modern car (rolling code) | `FAIL` (intended pedagogical fail) |
| IR receive | `THINKING` |
| IR receive got code | `HAPPY` |
| IR transmit | `WORKING` |
| SD missing | `FAIL` (status badge only, not a full-screen) |
| Battery <20% (post-§6.4 integration) | `SAD` (status badge), all radios still allowed |
| Battery <3.3 V cell — cutoff | `FAIL` (full-screen), radios halted, prompt to plug USB |
| Generic operation error | `FAIL` |

**Implementation rule.** Every screen calls `triggerReaction(MOOD_X, ...)` on entry and on each state transition. Stub call is acceptable for first integration; full per-mood animation polish is a Day 6+ task and may slip if sensor work is at risk.

**Gamification (defer / time-permitting).** Score, streak, mascot reactions to specific milestones (e.g. first FIAT unlock of the demo earns a unique mascot pose). These are Day 8–9 polish, not blockers. If screen real-estate (128×64) cannot fit a score line, drop the score and keep the mascot.

### 10.4 Marauder-inspired layout

The Wi-Fi feature menus take their layout cues from ESP32 Marauder (top title bar, scrollable list, bottom status line) but are drawn from scratch with original glyphs and the VariOne mascot. **No copied assets.**

---

## 11. Data Model — SD Capture Schemas

### 11.1 Sub-GHz capture (`subghz/*.json`)

```json
{
  "schema": 1,
  "captured_at": "2026-04-26T10:14:33Z",
  "freq_hz": 433920000,
  "modulation": "OOK",
  "data_rate_bps": 2400,
  "rssi_dbm": -52,
  "edges_us": [412, 824, 412, 412, ...],
  "sha1": "..."
}
```

### 11.2 NFC capture (`nfc/*.json`)

```json
{
  "schema": 1,
  "captured_at": "2026-04-26T10:15:01Z",
  "type": "ISO14443A",
  "uid": "04:A2:1B:7C:9D:6E:80",
  "sak": "0x20",
  "atqa": "0x0344",
  "emv": {
    "aid": "A0000000031010",
    "pan_masked": "************1234",
    "expiry": "12/27",
    "name": null
  },
  "sha1": "..."
}
```

The PAN is **never written to the file in clear** in MVP — only masked. Cardholder name, where present, is also masked to first-initial + last-name. This is a deliberate constraint above and beyond the consent framework, to harden against accidental SD loss during the demo.

### 11.3 IR capture (`ir/*.json`)

```json
{
  "schema": 1,
  "captured_at": "2026-04-26T10:16:11Z",
  "protocol": "NEC",
  "address": "0x04",
  "command": "0x05",
  "raw_us": [9000, 4500, 560, 560, ...],
  "sha1": "..."
}
```

---

## 12. Build and Deployment

- Repo private. Branches: `main` (stable demo), `dev` (integration), per-feature branches `feat/*`.
- CI not required for MVP; build verification via `pio run` locally.
- Releases tagged `v0.X.Y`; release artifacts are firmware `.bin` files committed to a private `releases/` directory.

---

## 13. Test Plan

### 13.0 Sensor-first smoke tests (mandatory before main.cpp integration)

**Rule.** No new peripheral, RF front end, or bus device is integrated into `main.cpp` until a standalone serial-only smoke test has run successfully on hardware and its result has been recorded in `docs/test-log.md`. This rule exists because the v0.4 CC1101 bring-up broke the OLED on shared SPI; the cost of catching such conflicts in a 30-line test sketch is far lower than catching them after integration.

Each smoke test lives in either:

- `test/test_<peripheral>.cpp` plus a dedicated PlatformIO env in `platformio.ini` (e.g. `[env:cc1101_test]`, `[env:pn532_test]`), OR
- A short branch with a temporary `src/main.cpp` replacement, **never committed to `main`**.

Each smoke test must:

1. Initialize **only** the peripheral under test plus serial.
2. Print a known-good signature on boot — chip ID, firmware version, register dump, I²C scan, etc. — so a missing/miswired part is unambiguous.
3. Loop a primitive sense or transmit operation with serial output every 1–2 s.
4. Run with **no edits to the working `main.cpp`** and no risk of regressing the v0.4 feature set.

**Minimum smoke-test set:**

| Peripheral | Expected signature | Loop op |
|---|---|---|
| CC1101 | `PARTNUM=0x00, VERSION=0x14` (or `0x04`) | Read RSSI on 433.92 MHz every 1 s |
| PN532 | I²C scan finds 0x24; firmware version reads | `readPassiveTargetID` poll, log UID on detect |
| microSD | mount succeeds, free MB prints | List `/` directory |
| IR RX (VS1838B) | none on boot | Decode and print 3 different remote presses |
| IR TX | LED blinks visibly under phone camera | Transmit a captured NEC code, target device responds |
| SAW filter / LNA chain | RSSI floor drops vs no-filter baseline | Compare RSSI on idle 433.92 MHz with and without inline |

Inconsistencies (bus contention, brownout, CS hygiene, shared-pin conflicts, address conflicts) **must** surface here, not at integration.

### 13.1 Unit-ish tests (host-side where possible)

- `EmvLite` parse: feed canned PPSE/AID responses, assert PAN/expiry extraction.
- `SubGhzCodec` edge encode/decode round-trip.
- Capture schema serialization round-trip via ArduinoJson.

### 13.2 Hardware bring-up checklist (Day 1–2)

1. ESP32 boots, serial console at 115200 prints version banner.
2. OLED renders splash.
3. All 4 buttons report distinct presses on serial.
4. SD card mounts, free space prints.
5. CC1101 reports correct partnum/version registers (0x00 / 0x14 expected).
6. PN532 firmware version readable.
7. IR receiver echoes a test remote press to serial.

### 13.3 Feature acceptance tests

Each feature in §9 has its acceptance criteria as the test script. Pass/fail recorded in `/docs/test-log.md`.

### 13.4 Demo dry runs

- Day 8: full S1–S4 dry run in lab.
- Day 9: full S1–S4 dry run in front of one non-team member.
- Day 10: live demo.

---

## 14. Project Plan — 10 Days, 5 People

### 14.1 Roles (one primary owner per area; pairs encouraged)

- **Engineer A — Hardware/RF lead:** wiring harness, PCB/perfboard, antennas, CC1101.
- **Engineer B — Firmware core:** main loop, SPI bus init, SD, OLED, menu, input.
- **Engineer C — NFC:** PN532 + EMV-lite parser.
- **Engineer D — Wi-Fi:** scan + deauth + evil twin + portal.
- **Engineer E — IR + Sub-GHz codec + integration testing + documentation.**

### 14.2 Schedule

| Day | Hardware (A) | Core (B) | NFC (C) | Wi-Fi (D) | IR/SubGHz/Test (E) |
|---|---|---|---|---|---|
| 1 | Wire harness, voltage check, CC1101 antenna swap | PlatformIO project skeleton, OLED splash, button scan, serial logger | PN532 I²C bring-up, UID read | Wi-Fi AP scan working | IR RX bring-up, document protocols seen |
| 2 | SD module wired and validated | SD mount, capture-store API, menu skeleton | EMV PPSE/AID parse against test card | Station scan in promiscuous mode | IR TX bring-up, NEC protocol round-trip |
| 3 | Enclosure CAD v1 sent to print | Menu navigation full, settings page | PAN/expiry parse + masking + SD write | Deauth frame crafting + injection | Sub-GHz codec stub, CC1101 register init |
| 4 | Enclosure fit check, antenna mount | Status LED patterns, error toast UI | NFC capture file format finalised | Deauth target-binding + safety guards | Sub-GHz capture (RAW) on bench transmitter |
| 5 | — | Logger to SD, system.log rotation | NFC done; supports E for sub-GHz schema parity | SoftAP rogue cloned SSID | Sub-GHz replay vs. FIAT key bench test |
| 6 | — | Performance pass (free RAM, loop latency) | Hardening + viva-prep notes | Captive portal HTML + DNS hijack | FIAT live test in lab parking |
| 7 | Final enclosure assembly | Integration freeze for menu | — | Portal credential capture to SD | IR universal remote presets bundled |
| 8 | — | All-feature dry run #1 | All-feature dry run #1 | All-feature dry run #1 | All-feature dry run #1, fix list |
| 9 | — | Dry run #2 with external observer | " | " | " |
| 10 | Demo logistics, cabling | Live demo support | Live demo support | Live demo support | Live demo + viva |

### 14.3 Daily ritual

15-minute standup at start of day. End-of-day commit + 5-line status note in `/docs/diary/DAY-N.md`.

---

## 15. Risks and Mitigations

| # | Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|---|
| RK1 | CC1101 SPI issues due to timing/CS hygiene | Low | Medium | **Largely mitigated.** SD card moved to dedicated SPI bus (GPIO 27/16/17/5), eliminating the CC1101/SD VSPI contention that was the original risk. CC1101 is now the sole device on VSPI. CS HIGH at boot still required. If a future peripheral is added to VSPI, re-introduce the SPI mutex |
| RK2 | FIAT 128 lock harder to capture than expected (signal short, room noisy) | Low | High | Capture in quiet RF environment first (anechoic-ish room or remote in direct contact); fallback to a bench-transmitter demo |
| RK3 | EMV parse fails on Egyptian card schemes (Meeza specifics) | Medium | Medium | Test against multiple test cards day 2–3; fall back to UID + ATQA display only for unsupported schemes |
| RK4 | Deauth ineffective because lab AP has PMF on | Medium | Medium | Provision the lab AP with WPA2 + PMF disabled on a **fixed known channel** (e.g. channel 6) before the demo. Document AP model, firmware version, and channel in the demo kit checklist. This channel also becomes the VariPortal SoftAP channel for same-channel concurrent operation (§9.6.1) |
| RK5 | VariPortal + deauth concurrent operation degrades portal responsiveness or drops client associations under load | Medium | Medium | Pin lab AP and SoftAP to the same channel to eliminate channel-hop overhead (§9.6.1). Use dual-core task pinning (portal on Core 0, deauth on Core 1). Benchmark with 3 simultaneous portal clients during deauth burst; if portal latency >2 s, reduce deauth duty cycle. Fall back to sequential demo (deauth → stop → VariPortal) only if same-channel concurrent operation is unreliable after tuning |
| RK6 | SD card corrupts mid-demo | Low | High | Keep a second pre-loaded SD card in the kit; capture-store writes atomic (write-then-rename) |
| RK7 | One team member out for any of 10 days | Medium | Medium | Pair every owner with a backup; daily commits enforce continuity |
| RK8 | Viva challenge: "does this perform unauthorized payments?" | High | High | §9.2 reality check is doctrine — every team member can answer this in one minute |
| RK9 | RF noise floor (sensors / mains / ISM clutter) swamps fixed-code car-key capture | Medium | High | §7.4 mitigation ladder: tighter CC1101 RX bandwidth → SAW bandpass → LNA. Bench-test in `docs/rf-frontend.md` before escalating |
| RK10 | Modern-car comparison set (§9.1.1) yields zero observable RF (smart-key UWB / BLE) — no negative-result data to show | Medium | Low | Pre-scan each modern fob in §13.0 phase to confirm it transmits *something* on 433/868 OOK before relying on it for the demo. Fall back to a desktop RTL-SDR waterfall screenshot if needed |
| RK11 | Mascot polish over-runs sensor work | Medium | Medium | Mascot calls are stubbed (single-line `triggerReaction()`) on every screen from Day 1. Animation polish is explicitly deferred to Day 8+ and may be cut. See §10.3 |
| RK12 | PN532 + OLED I²C bus contention under load | Low | Medium | Both devices already coexist on the bus in v0.4; if a load spike exposes contention, drop I²C clock to 100 kHz and serialize OLED writes around NFC polls. **Do not** revert PN532 to SPI — that is a larger rework |
| RK13 | Battery brownout under TX burst (boost converter sags during 240 mA Wi-Fi TX or 35 mA CC1101 TX on top of 150 mA idle) | Medium | High | Add 470 µF–1000 µF bulk cap on 5 V rail post-boost; benchmark cell voltage during deauth burst. Fall back to USB-C-powered demo if battery cannot hold ≥3.5 V cell during the worst-case TX moment in S4 |
| RK14 | Charge circuit not certified for simultaneous charge + load | Medium | Low | Treat device as charge-or-run; full-charge before each demo cycle. Document on the kit checklist |

---

## 16. Success Criteria

The MVP is a success on demo day if:

- All four scenarios S1–S4 run successfully in front of the review board.
- Every feature in §9 meets its acceptance criteria.
- The team can answer, without hesitation, the §9.2 reality check.
- The device persists captures across a hard reboot and replays them.
- The project earns a defensible mark and a green light from supervisors to proceed to the next phase (course curriculum partnership, expanded research).

---

## 17. Future Capabilities (Stretch — Not Confirmed for MVP)

The following features are under consideration for post-MVP development or as stretch goals if the MVP timeline permits. They are documented here so that architectural decisions in the MVP do not accidentally block them, and so that the documentation and course-curriculum teams can plan around them.

### 17.1 Cloud-based AI analysis pipeline (VariLearn)

**Concept.** Captured data (sub-GHz signal recordings, NFC card metadata, Wi-Fi reconnaissance logs, VariPortal credential submissions) is uploaded from the device to a cloud endpoint, where an AI model processes the raw captures into structured educational material — lesson summaries, annotated attack breakdowns, risk assessments, and student-facing lab worksheets.

**Why this matters.** The raw capture files (JSON edge arrays, hex UIDs, portal logs) are useful to the operator but meaningless to a student or a non-technical audience. An AI layer bridges that gap: it can take a sub-GHz capture and produce a narrative like "this 433.92 MHz OOK signal contains a 24-bit fixed code with no rolling component — here is why that is insecure, here is the countermeasure, here is what a secure implementation looks like." Scaling that across hundreds of captures and dozens of students is the difference between a one-off demo and a repeatable course curriculum.

**Proposed architecture.**

```
ESP32 (capture) → SD card (local store)
                      ↓
              [Sync mode — ESP32 connects to a known
               non-target Wi-Fi network and POSTs
               capture files to the cloud endpoint]
                      ↓
         Cloud API (REST, HTTPS, authenticated)
                      ↓
         AI model (e.g. Claude API, OpenAI, or
         self-hosted Ollama on a VPS)
                      ↓
         Processed output: lesson JSON, PDF worksheet,
         annotated capture summary
                      ↓
         Returned to operator via web dashboard or
         pulled back to SD on next sync
```

**ESP32 constraints and how to work around them.**

- **Radio sharing.** The ESP32's single 2.4 GHz radio cannot upload to the cloud while running Wi-Fi attacks (deauth, VariPortal). Upload must happen in a dedicated **sync mode** where the ESP32 connects to a known, non-target Wi-Fi network (e.g. the lab's infrastructure SSID, a phone hotspot, or a dedicated upload AP). The menu would expose this as `Settings → Cloud Sync` or similar. The device is either in attack mode or in sync mode, never both.
- **TLS/HTTPS memory overhead.** ESP32 has ~320 KB usable RAM. TLS handshakes for HTTPS consume ~40–60 KB. This is tight but feasible — the Arduino `WiFiClientSecure` library handles it, and Marauder already makes HTTPS calls for firmware updates on the same hardware. Keep the upload payload small (one capture file per request, not a batch dump).
- **Upload format.** POST the raw capture JSON files as-is (they already have schemas from §11). The cloud AI processes them server-side. The ESP32 does not run any AI inference locally — it is a capture-and-upload client only.
- **Authentication.** A device-specific API key stored in `/config.json` on the SD card, sent as a Bearer token in the upload header. Do not hardcode keys in firmware.
- **Offline-first.** The device works fully without cloud access. SD card remains the primary store. Cloud sync is additive — if the network is unavailable, captures stay on the SD card and sync later.

**AI model responsibilities.**

- **Signal analysis:** Parse sub-GHz edge arrays, identify modulation scheme (OOK, FSK), detect fixed-code vs. rolling-code patterns, estimate protocol (PT2262, EV1527, KeeLoq), generate a human-readable summary with security assessment.
- **NFC data annotation:** Take masked PAN + ATQA/SAK/AID metadata, explain what each field means, what an attacker could do with it, what the cardholder's countermeasures are (RFID-blocking sleeve, transaction alerts, contactless limit awareness).
- **Wi-Fi attack debrief:** Take VariPortal logs (timestamps, association events, credential submissions), reconstruct the attack timeline, produce a lesson narrative: "at T+0 the deauth began, at T+3 the victim's device probed for the SSID, at T+5 it associated with the clone, at T+8 credentials were submitted — here is where each defensive layer (PMF, HTTPS certificate pinning, 2FA) would have stopped the chain."
- **Output formats:** Structured JSON (for programmatic use), Markdown (for quick review), and optionally PDF (for printable student handouts).

**Open questions (to resolve before implementation).**

- Which AI provider? Claude API (Anthropic), OpenAI, or self-hosted (Ollama on the team's VPS)? Cost, latency, and data-residency implications differ.
- Does the NDA with partner organizations permit uploading captured data (even masked) to a third-party cloud? If not, the AI must run on infrastructure the team controls.
- Who builds and maintains the cloud backend? This is a separate engineering effort from the firmware — it needs its own owner.

### 17.2 Cloud capture storage (VariCloud)

**Concept.** In addition to the SD card, captured data is pushed to a cloud storage backend so that captures persist beyond the physical SD card, are accessible from any device, and can feed into the VariLearn AI pipeline (§17.1) or a web-based review dashboard.

**Proposed architecture.**

- **Storage backend:** Firebase Realtime Database or Firestore (free tier handles the expected volume), or a self-hosted REST API backed by PostgreSQL/SQLite on the team's VPS. Choose based on NDA constraints.
- **Sync trigger:** Same sync mode as §17.1 — the ESP32 connects to a non-target SSID and uploads. Captures are uploaded individually as they sync, with a local flag on the SD marking each file as synced/unsynced.
- **Conflict handling:** The SD card is the source of truth. Cloud is a replica. If a file exists on cloud but not on SD (SD was wiped or swapped), the cloud copy persists. If a file exists on SD but not on cloud (sync hasn't happened yet), it uploads on next sync.
- **Data retention:** Cloud copies are retained indefinitely for the course curriculum and research data set. SD card copies may be wiped between demo sessions at the operator's discretion.

**ESP32 limitations.**

- Same radio-sharing constraint as §17.1 — sync mode is exclusive with attack mode.
- Firebase client libraries exist for ESP32 (`Firebase-ESP32` by mobizt), but they add ~50–80 KB to the firmware binary. Budget this against the partition scheme.
- Upload speed on ESP32 over Wi-Fi is typically 1–5 Mbps effective throughput. A typical capture file is <10 KB. Syncing 50 captures takes seconds, not minutes.
- If using a custom REST API instead of Firebase, the ESP32 uses `HTTPClient` + `WiFiClientSecure` with a pinned server certificate.

**Relationship to VariLearn.** VariCloud is the storage layer; VariLearn is the processing layer. They share the same cloud endpoint and the same sync mode. The upload path is: ESP32 → VariCloud (store) → VariLearn (process) → output (lessons, reports). If VariLearn is not built yet, VariCloud still has standalone value as a backup and data-aggregation layer.

---

## 18. Glossary

- **AC** — Application Cryptogram (EMV)
- **AID** — Application Identifier (EMV)
- **ATQA** — Answer To Request, Type A (ISO 14443A)
- **EMV** — Europay/Mastercard/Visa contactless payment standard
- **PAN** — Primary Account Number
- **PMF** — Protected Management Frames (802.11w)
- **PPSE** — Proximity Payment System Environment (EMV)
- **PSK** — Pre-Shared Key (Wi-Fi)
- **SAK** — Select Acknowledge (ISO 14443A)
- **SoftAP** — ESP32 software access point mode
- **TSOP** — TSOP-series IR receiver IC family (Vishay)
- **VariPortal** — VariOne's cloned-AP + captive-portal feature (F6), formerly called "evil twin" in prior drafts
- **VariLearn** — Proposed cloud-based AI analysis pipeline that converts raw captures into educational material (§17.1, stretch)
- **VariCloud** — Proposed cloud capture storage backend (§17.2, stretch)

---

## Appendix A — Initial PlatformIO Project Skeleton

The following file/directory tree is created on Day 1. Every file starts with a one-paragraph header comment describing its responsibility.

```
varione/
├── platformio.ini
├── README.md                  (private — internal use only)
├── docs/
│   ├── PRD.md                 (this document)
│   ├── pinmap.md              (a copy of §7.2 for quick reference)
│   ├── test-log.md
│   └── diary/
├── src/
│   ├── main.cpp
│   ├── config.h
│   ├── core/
│   │   ├── SpiBus.h
│   │   ├── SpiBus.cpp
│   │   ├── EventLoop.h
│   │   ├── EventLoop.cpp
│   │   ├── Logger.h
│   │   ├── Logger.cpp
│   │   ├── StatusLed.h
│   │   └── StatusLed.cpp
│   ├── ui/
│   │   ├── Display.h
│   │   ├── Display.cpp
│   │   ├── Input.h
│   │   ├── Input.cpp
│   │   ├── MenuTree.h
│   │   ├── MenuTree.cpp
│   │   ├── Screens.h
│   │   └── Screens.cpp
│   ├── storage/
│   │   ├── SdCard.h
│   │   ├── SdCard.cpp
│   │   ├── CaptureStore.h
│   │   └── CaptureStore.cpp
│   ├── radio/
│   │   ├── SubGhz.h
│   │   ├── SubGhz.cpp
│   │   ├── SubGhzCodec.h
│   │   └── SubGhzCodec.cpp
│   ├── nfc/
│   │   ├── NfcReader.h
│   │   ├── NfcReader.cpp
│   │   ├── EmvLite.h
│   │   └── EmvLite.cpp
│   ├── ir/
│   │   ├── IrRx.h
│   │   ├── IrRx.cpp
│   │   ├── IrTx.h
│   │   └── IrTx.cpp
│   └── wifi/
│       ├── WifiScan.h
│       ├── WifiScan.cpp
│       ├── Deauth.h
│       ├── Deauth.cpp
│       ├── VariPortal.h
│       ├── VariPortal.cpp
│       ├── Portal.h
│       └── Portal.cpp
└── test/
    └── (host-side unit tests for codec/parsers)
```

## Appendix B — `config.h` first cut

```cpp
#pragma once

// ---- Pin map (single source of truth, matches PRD §7.2) ----
#define PIN_I2C_SDA       21
#define PIN_I2C_SCL       22

#define PIN_VSPI_SCK      18
#define PIN_VSPI_MISO     19
#define PIN_VSPI_MOSI     23

#define PIN_CC1101_CS     15
#define PIN_CC1101_GDO0    4
#define PIN_CC1101_GDO2   -1

#define PIN_PN532_IRQ     13  // wired but firmware polls; reserved for future IRQ-driven detection

#define PIN_SD_CS          5
#define PIN_SD_SCK        27  // dedicated SD SPI bus (not on VSPI)
#define PIN_SD_MOSI       17
#define PIN_SD_MISO       16

#define PIN_IR_TX         25
#define PIN_IR_RX         36

#define PIN_BTN_UP        26
#define PIN_BTN_DOWN      33
#define PIN_BTN_SELECT    32
#define PIN_BTN_BACK      14

#define PIN_LED_STATUS     2

// ---- Defaults ----
#define DEAUTH_DEFAULT_SECONDS    15
#define DEAUTH_MAX_SECONDS        60
#define SUBGHZ_DEFAULT_HZ         433920000UL
#define IR_CARRIER_HZ             38000

// ---- Build info ----
#define VARIONE_VERSION    "0.1.0"

// ---- Safety guards ----
#define ALLOW_BROADCAST_DEAUTH    1   // operator-selectable; lab environments are authorized
#define VARIPORTAL_MAX_CLIENTS    8   // SoftAP concurrent client limit
#define DEAUTH_PORTAL_COEXIST     1   // allow F5+F6 simultaneous operation
```

---

*End of document. Revision history: v0.1 — initial draft for supervisor review.*
