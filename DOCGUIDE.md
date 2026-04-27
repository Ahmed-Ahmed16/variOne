# VariOne — Documentation Guide

**Purpose:** This file is a reference for the team member responsible for
writing the full graduation project documentation. Every chapter and
subsection of the required document structure is mapped below to the exact
PRD sections, repo files, and data sources where the raw information lives.

**Rule:** Do not invent information. Everything in the documentation must
trace back to a source listed here. If a source does not exist yet (marked
**[PENDING]**), flag it to the team — someone needs to produce the artifact
before the section can be written.

---

## Chapter One: Introduction

### 1.1 Problem Definition

**What to write:** Egypt's wireless security posture is weak in three
specific, measurable areas — Wi-Fi (WPA2 dominance, no PMF), vehicles
(fixed-code sub-GHz remotes), and contactless payments (EGP 600 PIN-less
limit with zero public awareness of data leakage). The pain is invisible
to non-technical people. VariOne makes it physically visible.

**Sources:**
- PRD §2 (Background and Motivation) — the three bullet points on Wi-Fi,
  vehicles, and contactless payments are the problem definition, with
  specific local-market context.
- PRD §2 final paragraph — the core insight ("a working demo changes minds
  in 30 seconds").
- PRD §1 (Executive Summary) — the two-outcome framing (academic +
  public-good).

### 1.2 Problems in the Existing System

**What to write:** There is no affordable, integrated, locally-relevant
security awareness demonstrator in the Egyptian market. Existing tools
(Flipper Zero, HackRF One, ESP32 Marauder) are either expensive, single-
purpose, open-source with no educational narrative, or not available
locally. None of them are framed as awareness instruments; they are pentest
tools for specialists.

**Sources:**
- PRD §2.1 (Prior Art and References) — Marauder, Flipper Zero, Samy
  Kamkar's work. Use these as the "existing system" comparisons.
- Market research: price Flipper Zero and HackRF One in EGP on local
  Egyptian retailers (Future Electronics, RAM Electronics, Sigma
  Electronics, AliExpress with Egypt shipping). Compare to VariOne's
  BOM cost in PRD §7.1 (sub-USD 100 / ~EGP 5000 total for all
  components).
- **[PENDING]** If the team has conducted any survey, interview, or
  focus-group data with CIC students or partner organizations about their
  current security awareness level, include it here. If not, reference the
  general market observation from PRD §2.

### 1.3 The Proposed Information System

**What to write:** VariOne — a handheld, ESP32-based wireless security
awareness platform that combines four attack surfaces (sub-GHz, NFC, IR,
Wi-Fi) into a single device, operated in controlled environments under
academic supervision, to make abstract security risks physically visible.

**Sources:**
- PRD §1 (Executive Summary) — the one-paragraph product description.
- PRD §3.1 (Goals, In-Scope) — the G1–G8 capability table. This IS the
  system's feature list.
- PRD §4.3 (Demo Scenarios S0–S4) — describe each scenario as a use
  case of the proposed system.
- PRD §6.1 (Architecture Diagram) — include the textual architecture
  diagram as a system overview figure.

### 1.4 Objectives

**What to write:** List the project objectives as measurable outcomes.

**Sources:**
- PRD §16 (Success Criteria) — these five bullets are the project
  objectives verbatim.
- PRD §3.3 (Explicit Research Questions R1–R3) — these are the research
  objectives.
- PRD §1 — the two-outcome framing (academic graduation + public-good
  awareness).

### 1.5 Methodology (System Development Approach / SDLC)

**What to write:** The team uses an **iterative prototyping** approach,
not waterfall. The 10-day sprint is structured around daily integration
and hardware-first validation. Each feature has explicit acceptance
criteria and is tested independently before integration.

**Sources:**
- PRD §14 (Project Plan) — the 10-day schedule with daily standups and
  end-of-day commits.
- PRD §14.3 (Daily Ritual) — standup + diary note = iterative review
  cycle.
- PRD §13.0 (Sensor-First Smoke Tests) — the mandatory test-before-
  integrate rule. This is the team's validation methodology.
- PRD §13.2 (Hardware Bring-Up Checklist) — Day 1 validation steps.
- PRD §13.4 (Demo Dry Runs) — Days 8–10 acceptance testing.
- Repo: `docs/diary/DAY-N.md` files **[PENDING — these should exist as
  daily logs]**.
- Repo: `docs/test-log.md` **[PENDING — smoke test results go here]**.

### 1.6 Project Organization

**What to write:** Team of 5 engineers, each with a primary ownership area,
supervised by Dr. Ahmed Gaber (Networks Department, CIC New Cairo). Pair
each owner with a backup.

**Sources:**
- PRD §14.1 (Roles) — Engineers A–E with responsibilities.
- PRD §14.2 (Schedule) — who does what on which day.
- PRD §5.1 (Authorization) — supervisor and institutional context.

---

## Chapter Two: Planning Phase

### 2.1 Introduction

**What to write:** Brief overview of the planning approach — constrained
by a 10-day build window, 5 people, sub-USD 100 BOM budget, and three
authorized test environments.

**Sources:**
- PRD §14 (Project Plan) — the constraints.
- PRD §5.1 (Authorization) — the three environments.

### 2.2 Problem Scope

**What to write:** What is in scope vs. out of scope for the MVP.

**Sources:**
- PRD §3.1 (In-Scope) — the G1–G8 table.
- PRD §3.2 (Out-of-Scope) — the explicit exclusions (rolling-code
  attacks, WPA3, EMV replay, Bluetooth, open-source release, mass
  production).
- PRD §17 (Future Capabilities) — VariLearn and VariCloud as post-MVP
  stretch goals.

### 2.3 Project Schedule

**Sources:**
- PRD §14.2 (Schedule) — the 10-day table. Reproduce this as the
  schedule.
- PRD §14.3 (Daily Ritual) — the standup + diary process.

### 2.3.1 Gantt Chart

**What to produce:** Convert the PRD §14.2 table into a Gantt chart.

**Sources:**
- PRD §14.2 — each cell is a task. Map Engineer A–E to rows, Days 1–10
  to columns. Color-code by feature area (Wi-Fi = blue, NFC = green,
  Sub-GHz = orange, IR = red, Core = gray).
- **[PENDING]** The actual Gantt chart needs to be drawn (use Excel,
  MS Project, or a free tool like GanttProject). The PRD provides the
  raw data; the Gantt is a visual artifact the doc writer produces.

### 2.4 Staff the Project

**Sources:**
- PRD §14.1 (Roles) — Engineer A through E with areas.
- Add: Dr. Ahmed Gaber as Faculty Supervisor.
- Add: the documentation owner as a named role.

### 2.5 Feasibility Study

### 2.5.1 Types

**What to write:** Cover technical, economic, operational, and schedule
feasibility.

**Sources for each type:**

**Technical feasibility:**
- PRD §6 (System Architecture) — the hardware is proven (ESP32 + CC1101 +
  PN532 + IR + SD all working on the v0.4 prototype).
- PRD §2.1 (Prior Art) — ESP32 Marauder proves the Wi-Fi attack chain
  works on this hardware. Flipper Zero proves the multi-surface concept
  works as a product class.
- PRD §8.1 (Toolchain) — PlatformIO + Arduino-ESP32 is a mature, well-
  documented stack.
- PRD §9.6.1 (Single-Radio Concurrent Operation) — documents the
  technical challenge and solution for the hardest feature.

**Economic feasibility:** see §2.6 below.

**Operational feasibility:**
- PRD §5 (Legal, Ethical, and Operational Framework) — the authorization
  structure, Law 175 awareness, and data handling constraints prove the
  project can legally operate.
- PRD §4 (Users and Scenarios) — the operator model is simple (one team
  member runs the device).

**Schedule feasibility:**
- PRD §14.2 — the 10-day plan fits the scope because features are
  independent and parallelizable across 5 engineers.

### 2.6 Project Feasibility Study

### 2.6.1 Project Cost

**A. Development cost:**
- PRD §14 — 5 engineers × 10 days. Quantify in person-hours (assume ~8 h/
  day = 400 person-hours). If the university requires a monetary figure,
  use a nominal student-engineer rate (check with the faculty for the
  expected format).

**B. Hardware cost:**
- PRD §7.1 (BOM) — the full bill of materials with quantities. Price
  each item in EGP from local Cairo sources (Sigma Electronics, Future
  Electronics, RAM Electronics, AliExpress). The PRD already gives Cairo
  BOM estimates for the battery subsystem (~200–270 EGP in §6.4).
- **[PENDING]** A complete priced BOM spreadsheet with vendor, unit cost,
  and total. The PRD has the parts list; the doc writer needs to add
  current EGP prices.

**C. Software cost:**
- PlatformIO: free.
- VS Code: free.
- Arduino-ESP32 framework: free / open-source.
- All libraries in `platformio.ini` §8.1: free / open-source.
- Total software cost: **EGP 0**.

**D. Operational cost:**
- Electricity for development laptops and bench power supply: negligible.
- 3D printing: one enclosure. Get a quote from the team's printing
  provider.
- SD cards: ~EGP 50–100 each, 2 needed (primary + backup per PRD RK6).
- Lab access: provided by CIC at no charge.
- **[PENDING]** If VariLearn / VariCloud (§17) are pursued post-MVP, cloud
  hosting costs apply (Firebase free tier, VPS ~$5–10/month). Document
  this as future operational cost, not MVP cost.

### 2.6.2 The Benefit

**What to write:** Quantify benefits in terms of awareness reach (number
of students, number of demo sessions), academic output (graduation project
grade, potential publication), and partnership value (course curriculum
with the cybersecurity partner, government alignment).

**Sources:**
- PRD §1 — the two outcomes.
- PRD §4.2 (Audience) — faculty, government/corporate stakeholders,
  university students.
- PRD §16 (Success Criteria) — "green light from supervisors to proceed
  to the next phase (course curriculum partnership, expanded research)."
- **[PENDING]** Estimate the number of students who could be trained per
  semester using VariOne in the CIC Networks Department lab.

### 2.6.3 Equilibrium (Breakeven) Point

**What to write:** Since this is an academic/non-commercial project, the
breakeven is defined as: the point at which the project's educational
output (students trained, awareness sessions delivered) justifies the
hardware and development investment. Calculate: total cost ÷ cost per
awareness session = number of sessions to break even. If the project
transitions to a commercial course curriculum with the cybersecurity
partner, calculate: course revenue per student × students per cohort ÷
total cost.

**Sources:**
- Cost data from §2.6.1 above.
- **[PENDING]** Revenue estimate if course curriculum is commercialized
  (this depends on the partner agreement — may not be available under
  NDA).

### 2.7 SWOT

**What to write:** Strengths, Weaknesses, Opportunities, Threats.

**Sources:**

- **Strengths:** PRD §1 (unique local-market positioning), PRD §7.1 (low
  BOM cost), PRD §6 (proven hardware platform), PRD §5 (government-backed
  authorization), PRD §2.1 (Marauder reference validates technical
  approach).
- **Weaknesses:** PRD §3.2 (out-of-scope limitations — no rolling-code,
  no WPA3, no Bluetooth), PRD §9.6.1 (single-radio constraint), PRD §15
  (risk register — each risk is a weakness).
- **Opportunities:** PRD §16 (course curriculum partnership), PRD §17
  (VariLearn AI pipeline, VariCloud), expansion to other universities
  and government agencies.
- **Threats:** PRD §15 (risk register), PRD §5.2 (Law 175 — legal
  exposure if operated outside authorized environments), competition from
  imported tools (Flipper Zero availability in Egypt).

---

## Chapter Three: System Analysis

### 3.1 Definition and Importance of System Analysis

**What to write:** Standard academic definition of system analysis, then
explain how it applies to VariOne: the team analyzed the Egyptian wireless
security landscape, identified the gap (no local awareness tool), gathered
requirements from the faculty supervisor and the cybersecurity partner, and
produced a PRD as the requirements specification.

### 3.2 Data Collection and Requirements Gathering

**What to write:** How requirements were gathered.

**Sources:**
- The original Claude chat (the shared conversation at
  `claude.ai/share/7dfddebb-...` or the `Claude.html` backup) — this IS
  the requirements elicitation session. The user (Ahmed) described the
  project scope, the faculty supervisor's requirements, and the
  cybersecurity partner's feature requests (Wi-Fi deauth + cloned AP).
  Claude asked clarifying questions. Ahmed answered. The PRD was produced
  from those answers.
- PRD §4.3 (Demo Scenarios) — these scenarios came from the partner
  requirements.
- PRD §5.1 — the three authorized environments came from signed
  agreements.
- **[PENDING]** If the team has meeting notes, emails, or chat logs from
  requirements sessions with Dr. Ahmed Gaber or the cybersecurity partner,
  reference them here (without disclosing NDA-protected details).

### 3.3.1 Tools

**What to write:** Tools used for analysis and development.

**Sources:**
- PRD §8.1 (Toolchain) — PlatformIO, VS Code, Arduino-ESP32.
- PRD `platformio.ini` library list — U8g2, PN532, SmartRC-CC1101,
  IRremoteESP8266, ArduinoJson.
- Repo structure — GitHub for version control.
- Claude AI — used as a technical co-pilot for PRD generation and
  firmware development.
- Fritzing or KiCad — if wiring diagrams were produced **[PENDING]**.

### 3.4 Project Requirements

**A. Functional Requirements:**
- PRD §3.1 (Goals G1–G8) — each goal IS a functional requirement.
- PRD §9 (Feature Specifications F1–F8) — each feature's "Purpose" and
  "Technical approach" sections expand the functional requirements.
- PRD §9 acceptance criteria — these are the testable functional
  requirements.
- Summarize as a numbered requirements table:
  - FR1: The system shall capture and replay fixed-code sub-GHz signals
    (→ PRD G1, F1, §9.1).
  - FR2: The system shall read NFC/RFID cards at ISO 14443A (→ PRD G2,
    F2, §9.2).
  - FR3: The system shall receive and transmit IR signals (→ PRD G3, F3,
    §9.3).
  - FR4: The system shall scan Wi-Fi APs and stations (→ PRD G4, F4,
    §9.4).
  - FR5: The system shall deauthenticate Wi-Fi clients in three targeting
    modes (→ PRD G5, F5, §9.5).
  - FR6: The system shall clone an AP with a credential-capture portal
    (VariPortal), running concurrently with deauth (→ PRD G6, F6, §9.6).
  - FR7: The system shall persist all captures to SD card (→ PRD G7, F7,
    §9.7).
  - FR8: The system shall provide OLED menu UI with 4-button navigation
    (→ PRD G8, F8, §9.8).

**B. Non-Functional Requirements:**
- **Performance:** PRD §9.4 AC1 (scan ≥10 APs in 5 s), PRD §9.5 AC1
  (deauth within 5 s), PRD §9.6 AC1 (portal redirect within 10 s).
- **Portability:** PRD §6.4 (battery operation, handheld form factor).
- **Usability:** PRD §10 (OLED menu, mascot system, 4-button navigation).
- **Security/Ethics:** PRD §5 (authorization framework, Law 175, data
  handling).
- **Reliability:** PRD §15 RK6 (atomic SD writes, backup SD card).
- **Maintainability:** PRD §8.2 (modular codebase, one class per
  peripheral).

### 3.5 Data Flow Diagrams (DFD)

**What to produce:** Draw DFDs for the system.

**Sources for DFD construction:**
- **Context diagram (Level 0):** External entities = Operator, Target
  Device (car, phone, card, TV/AC), SD Card, OLED Display, Cloud
  (future). Process = VariOne System. Data flows = RF signals in/out,
  NFC data in, IR signals in/out, Wi-Fi frames in/out, capture files to
  SD, UI commands from buttons, display data to OLED.
- **Level 1:** Decompose into the module structure from PRD §8.2 —
  SubGhz, NfcReader, IrRx/IrTx, WifiScan, Deauth, VariPortal, Portal,
  CaptureStore, MenuTree, Display.
- **Level 2 (if needed):** Decompose individual modules. For example,
  the Wi-Fi attack flow: WifiScan → (AP list) → Deauth → (frames) →
  VariPortal → (SoftAP) → Portal → (credentials) → CaptureStore →
  (file) → SD.

### 3.6 Use Case Diagram

**What to produce:** UML use case diagram.

**Sources:**
- **Actor:** Operator (the team member running the device).
- **Use cases:** Map directly from PRD §4.3 demo scenarios:
  - UC1: Boot device (S0)
  - UC2: Capture and replay sub-GHz signal (S1)
  - UC3: Read NFC bank card (S2)
  - UC4: Capture and replay IR signal (S3)
  - UC5: Scan Wi-Fi networks
  - UC6: Deauthenticate Wi-Fi clients (S4 phase 1)
  - UC7: Run VariPortal with credential capture (S4 phase 2)
  - UC8: Browse and replay saved captures
  - UC9: Change settings (deauth timeout, portal theme, etc.)
  - UC10: Sync captures to cloud (future — §17)
- **Includes/Extends:** UC6 extends UC5 (must scan before deauth). UC7
  extends UC5 (must select AP before cloning). UC7 includes UC6 (deauth
  runs concurrently during VariPortal).

---

## Chapter Four: System Design

### 4.1 Definition and Importance of System Design

**What to write:** Standard academic definition, then explain that
VariOne's design is driven by the PRD — hardware architecture first
(pinmap, bus topology, voltage), then software architecture (module
breakdown, cooperative scheduler, capture schemas).

### 4.2 Types of System Design

**What to write:** Distinguish between logical design (PRD §8 software
architecture, module breakdown) and physical design (PRD §6 hardware
architecture, §7 BOM and pinmap, §6.4 battery).

### 4.3 Entity Relationship Diagram (ERD)

**What to produce:** ERD for the data stored by the system.

**Sources:**
- PRD §11 (Data Model — SD Capture Schemas) — these JSON schemas define
  the entities and their attributes.

**A. Entities:**
- `SubGhzCapture` — a captured sub-GHz signal.
- `NfcCapture` — a captured NFC card read.
- `IrCapture` — a captured IR signal.
- `PortalSubmission` — a credential submission from the VariPortal.
- `SystemConfig` — the device configuration.
- `PortalTheme` — a captive portal theme (generic or target-specific).

**B. Attributes:**
- `SubGhzCapture`: schema, captured_at, freq_hz, modulation, data_rate_bps,
  rssi_dbm, edges_us[], target_class, sha1. (PRD §11.1)
- `NfcCapture`: schema, captured_at, type, uid, sak, atqa, emv{aid,
  pan_masked, expiry, name}, sha1. (PRD §11.2)
- `IrCapture`: schema, captured_at, protocol, address, command, raw_us[],
  sha1. (PRD §11.3)
- `PortalSubmission`: timestamp, cloned_ssid, client_mac, submitted_username,
  submitted_password, theme_name. (inferred from PRD §9.6 + §9.7)
- `SystemConfig`: brightness, default_freq, deauth_timeout_s, deauth_mode,
  active_portal_theme, cloud_api_key (future). (inferred from PRD §9.7
  `/config.json`)
- `PortalTheme`: name, description, path. (from PRD §8.4 `theme.json`)

**C. Relationships:**
- SubGhzCapture, NfcCapture, IrCapture are independent entities (no FK
  relationships — each capture is standalone).
- PortalSubmission references the VariPortal session (cloned_ssid +
  timestamp).
- PortalTheme is referenced by SystemConfig (active_portal_theme).

**Note:** VariOne uses flat-file JSON on an SD card, not a relational
database. The ERD is a logical model for documentation purposes. The
physical storage is the SD directory layout in PRD §9.7.

### 4.4 Physical Database Design

**What to write:** The SD card directory structure IS the physical
database. Each capture type has its own directory; each capture is a
JSON file named by timestamp.

**Sources:**
- PRD §9.7 — the directory layout.
- PRD §11 — the JSON schemas.

### 4.5 Class Diagram

**What to produce:** UML class diagram.

**Sources:**
- PRD §8.2 (Module Breakdown) — each `.{h,cpp}` pair is a class.
  The diagram should show:
  - `SpiBus` (shared by SubGhz, SdCard)
  - `Display` (wraps U8g2)
  - `Input` (4-button debounce)
  - `MenuTree` (references Display, Input)
  - `Screens` (references Display, MenuTree, all feature modules)
  - `SdCard` (holds SPI mutex reference)
  - `CaptureStore` (uses SdCard)
  - `SubGhz` (holds SPI mutex reference, uses CaptureStore)
  - `SubGhzCodec` (used by SubGhz)
  - `NfcReader` (uses Wire/I2C)
  - `EmvLite` (used by NfcReader)
  - `IrRx`, `IrTx` (GPIO-based)
  - `WifiScan` (uses ESP32 Wi-Fi API)
  - `Deauth` (uses WifiScan for targets)
  - `VariPortal` (uses WifiScan for SSID clone, coordinates with Deauth)
  - `Portal` (HTTP server, DNS, theme loader; used by VariPortal)
  - `Logger` (uses SdCard and Serial)
  - `StatusLed` (GPIO 2)
- Repo: `src/` directory structure.

### 4.6 Sequence Diagram

**What to produce:** UML sequence diagrams for the key interaction flows.

**Recommended diagrams (one per demo scenario):**

- **S1 (Sub-GHz capture and replay):** Operator → Input → MenuTree →
  SubGhz.startCapture() → CC1101 → GDO0 edges → SubGhzCodec.decode() →
  CaptureStore.save() → SD → [later] → SubGhz.replay() → CC1101 TX →
  target door unlocks.
- **S2 (NFC bank card read):** Operator → Input → MenuTree →
  NfcReader.poll() → PN532 → card detected → EmvLite.parsePPSE() →
  EmvLite.readPAN() → Display.showMaskedPAN() → CaptureStore.save() →
  SD.
- **S4 (VariPortal full chain):** Operator → WifiScan.scanAPs() →
  select target → VariPortal.start(ssid, channel) → SoftAP up →
  Portal.startServer() → Deauth.start(target, mode) → deauth frames →
  victim device deauths → victim probes → victim associates with SoftAP
  → DNS redirect → Portal serves page → victim submits creds →
  CaptureStore.save() → Display shows masked creds.

### 4.7 Activity Diagram

**What to produce:** Activity diagrams for operator workflows.

**Sources:**
- PRD §9.5 UX flow: `Wi-Fi → Deauth → Pick AP → Scan Stations → Pick
  Mode → Run`.
- PRD §9.6 UX flow: `Wi-Fi → VariPortal → Pick SSID → Pick Theme →
  Start`.
- PRD §9.1 UX flow: `Sub-GHz → Read RAW` / `Sub-GHz → Saved → Replay`.
- PRD §9.2 UX flow: `NFC → Read Card` / `NFC → Read Bank Card`.

### 4.8 Design Application

**What to write:** The design principles and patterns used.

**Sources:**
- PRD §8.3 (Cooperative Scheduling) — 1 ms tick cooperative scheduler,
  non-blocking operations, BACK-to-cancel.
- PRD §8.2 — one class per peripheral, SPI mutex pattern.
- CLAUDE.md — coding conventions (no global state, dependencies via
  constructors, English comments).
- PRD §9.6.1 — dual-core task allocation pattern (Core 0 = network,
  Core 1 = UI + injection).

### 4.9 Design User Interfaces

**What to produce:** UI mockups or wireframes for the OLED screens.

**Sources:**
- PRD §10.1 (Screen Taxonomy) — splash, main menu, list screens, action
  screens, confirmation screens.
- PRD §10.2 (Status Indicators) — LED patterns, OLED top-right corner
  status.
- PRD §10.3 (Mascot System) — mood bindings per screen.
- PRD §10.4 (Marauder-Inspired Layout) — top title bar, scrollable list,
  bottom status line.
- PRD §9.8 (F8 Menu UI) — 128×64, 5 visible rows, SD mount state + free
  RAM in status bar.
- **[PENDING]** Actual OLED screenshots or pixel-art mockups. These
  should come from the working firmware on the device — photograph or
  screenshot the real screens.

---

## Chapter Five: System Implementation

### 5.1 The Used Programs

**What to write:** List all tools and technologies.

**Sources:**
- PRD §8.1 (Toolchain) — PlatformIO, VS Code, Arduino-ESP32 framework,
  ESP32 board package.
- PRD `platformio.ini` (Appendix B / §8.1) — full library dependency
  list with version pins.
- Repo: `platformio.ini` — the live, current dependency list.
- 3D printing software (whichever the team used for the enclosure).
- GitHub — repository hosting.
- Claude AI — technical co-pilot.

### 5.2 Screenshots of System

**What to produce:** Screenshots or photographs of every user-facing
screen and the physical device.

**Sources:**
- **[PENDING]** Photograph the physical prototype: top view, side view,
  enclosure open, enclosure closed, with and without battery, connected
  via USB-C.
- **[PENDING]** Photograph or screenshot every OLED screen: splash, main
  menu, each sub-menu, each action screen in progress (scan running,
  deauth running, VariPortal running, NFC reading, IR capturing, Sub-GHz
  capturing), each result screen (card data, captured signal, portal
  submission).
- **[PENDING]** Screenshot of the captive portal page as seen on a phone
  browser — both generic themes and any target-specific themes.
- **[PENDING]** Serial monitor output showing boot sequence, sensor
  signatures, and capture logs.

### 5.3 Codes of the Important Project Modules

**What to include:** Key code excerpts (not the entire codebase).

**Sources — include excerpts from these files with explanations:**
- `src/config.h` — pin definitions, safety constants, version (PRD
  Appendix B).
- `src/core/SpiBus.cpp` — the SPI mutex pattern (PRD §8.2).
- `src/wifi/Deauth.cpp` — the deauth frame construction and
  `esp_wifi_80211_tx()` injection (PRD §9.5).
- `src/wifi/VariPortal.cpp` — SoftAP setup, concurrent deauth
  coordination, dual-core task pinning (PRD §9.6, §9.6.1).
- `src/wifi/Portal.cpp` — captive portal HTTP server, DNS hijack, theme
  loading from SD (PRD §8.4, §9.6).
- `src/nfc/EmvLite.cpp` — PPSE/AID parse, PAN extraction and masking
  (PRD §9.2).
- `src/radio/SubGhz.cpp` — CC1101 capture and replay (PRD §9.1).
- `src/radio/SubGhzCodec.cpp` — OOK edge encoding/decoding (PRD §9.1).
- Repo: the actual `src/` files once the revamp is complete.

---

## Chapter Six: System Testing

### 6.1 Types of Testing

### 6.1.1 Unit Testing

**Sources:**
- PRD §13.1 (Unit-ish Tests) — EmvLite parse, SubGhzCodec round-trip,
  capture schema serialization.
- Repo: `test/` directory.
- **[PENDING]** Actual test results — pass/fail, observed values.

### 6.1.2 Integration Test

**Sources:**
- PRD §13.0 (Sensor-First Smoke Tests) — the mandatory standalone test
  for each peripheral before integration. The smoke test table lists
  CC1101, PN532, microSD, IR RX, IR TX, SAW filter/LNA chain.
- PRD §13.2 (Hardware Bring-Up Checklist) — 7-step Day 1 integration
  test.
- Repo: `docs/test-log.md` **[PENDING — must be filled in with actual
  results]**.

### 6.1.3 Acceptance Test

**Sources:**
- PRD §13.3 (Feature Acceptance Tests) — every acceptance criterion in
  §9 IS an acceptance test case. List each one with its pass/fail result.
- PRD §13.4 (Demo Dry Runs) — Days 8–9 dry runs are the system-level
  acceptance tests.
- PRD §16 (Success Criteria) — the five top-level acceptance conditions.
- **[PENDING]** Actual test results from dry runs and demo day.

---

## Chapter Seven: Conclusion and Future Work

### 7.1 Conclusion

**Sources:**
- PRD §16 (Success Criteria) — state which criteria were met.
- PRD §3.3 (Research Questions R1–R3) — summarize findings.
- PRD §9.2 (NFC Reality Check) — this is a key finding to highlight
  (what the team expected vs. what is technically true about contactless
  EMV).
- PRD §9.1 (Sub-GHz Reality Check + §9.1.1 Modern Vehicle Comparison) —
  the positive-vs-negative result data set.
- **[PENDING]** Actual demo-day outcomes.

### 7.2 Future Work

**Sources:**
- PRD §3.2 (Out-of-Scope) — rolling-code attacks, WPA3, Bluetooth, EMV
  replay, open-source release — each is a candidate for future work.
- PRD §17.1 (VariLearn) — AI-powered educational material generation.
- PRD §17.2 (VariCloud) — cloud capture storage.
- PRD §6.4 (Battery) — if not completed in MVP, battery integration is
  future work.
- Course curriculum partnership with the cybersecurity partner (mention
  without disclosing NDA details).
- Expansion to other universities and government agencies.
- Custom PCB (replace perfboard/breadboard prototype).
- Mobile companion app for viewing captures and VariLearn output.

---

## III — References

**What to include:**
- PRD §2.1 (Prior Art) — ESP32 Marauder repo, Samy Kamkar's work,
  Flipper Zero.
- IEEE 802.11 standard (for deauth frame structure).
- ISO/IEC 14443A (for NFC).
- EMV Contactless Specifications (for the PPSE/AID/PAN parse).
- Egypt Law 175 of 2018 (Anti-Cyber and Information Technology Crimes
  Law).
- ESP-IDF documentation (`esp_wifi_80211_tx()` API reference).
- Datasheets: CC1101 (Texas Instruments), PN532 (NXP), SH1106 (Sino
  Wealth), ESP32-WROOM-32D (Espressif).
- IRremoteESP8266 library documentation.
- SmartRC-CC1101-Driver-Lib documentation.
- U8g2 library documentation.
- ArduinoJson library documentation.
- Adafruit PN532 library documentation.
- Any academic papers the team referenced during development.

---

## Artifact Checklist

Items marked **[PENDING]** above are artifacts that must exist before the
documentation can be completed. Summary:

| Artifact | Owner | Status |
|---|---|---|
| `docs/diary/DAY-1.md` through `DAY-10.md` | All engineers | **[PENDING]** |
| `docs/test-log.md` (smoke test results) | Engineer E | **[PENDING]** |
| Priced BOM spreadsheet (EGP) | Engineer A | **[PENDING]** |
| Gantt chart (visual) | Doc writer | **[PENDING]** |
| OLED screen photographs | Doc writer + Engineer B | **[PENDING]** |
| Physical prototype photographs | Engineer A | **[PENDING]** |
| Captive portal phone screenshots | Engineer D | **[PENDING]** |
| Serial monitor boot log | Engineer B | **[PENDING]** |
| Demo dry-run results (Day 8–9) | All engineers | **[PENDING]** |
| Demo-day results | All engineers | **[PENDING]** |
| Wiring diagram (Fritzing/KiCad) | Engineer A | **[PENDING]** |
| Survey/interview data (if any) | Doc writer | **[PENDING / optional]** |
