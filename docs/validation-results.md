# C5 EBCM Emulator Bench Validation

## Test Context

- Platform target: Corvette C5 (1997-2000), Class 2 VPW
- Emulator firmware: `firmware/src/main.ino`
- Profile used: `firmware/src/ebcm_profiles/c5_97_00.h`
- Note: Placeholder IDs/payload bytes must be replaced with capture-derived values before final pass/fail sign-off.

## Pre-Validation Setup

- Confirm wiring per `docs/bench-harness.md`
- Confirm capture-derived IDs/services/payloads are entered in profile header
- Confirm bus activity visible in sniffer
- Confirm no stuck bus state before power cycle tests

## Bench Validation Matrix

| Test ID | Procedure | Expected Result | Status | Notes |
|---|---|---|---|---|
| VAL-01 | Cold power cycle cluster + emulator 5 times | Cluster boots each cycle without EBCM comm loss indicator | Pending | Run after ID map finalized |
| VAL-02 | 10-minute steady-state run | No new EBCM comm warnings; heartbeat timing stable | Pending | Track dropped/missed frames |
| VAL-03 | Remove emulator TX for timeout window, then restore | Cluster flags comm loss during outage and recovers after restore | Pending | Confirms timeout behavior |
| VAL-04 | Send DTC status request stimulus (or natural request) | Emulator returns no-active-code response payload | Pending | Verify payload bytes against capture |
| VAL-05 | Rapid power cycle (3 cycles within 30 s) | Emulator resumes normal cadence every boot | Pending | Observe startup latency |

## Power-Cycle Log Template

| Cycle | Cluster Boot OK | EBCM Comm Warning | Recovery Time (s) | Notes |
|---|---|---|---:|---|
| 1 |  |  |  |  |
| 2 |  |  |  |  |
| 3 |  |  |  |  |
| 4 |  |  |  |  |
| 5 |  |  |  |  |

## Runtime Counters to Record

Record once per minute:

- `idsConfigured`
- `tx` counter
- `rx` counter
- observed warning state on cluster

## Sign-off Criteria

Validation is considered passed when:

- All matrix rows are marked Pass
- Cluster remains stable for the full steady-state run
- DTC/status behavior matches expected no-active-code semantics
- Repeated boot and timeout recovery are deterministic
