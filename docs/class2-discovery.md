# C5 EBCM Class 2 Discovery Workflow

## Objective

Determine the minimum set of J1850 VPW Class 2 frames required for a 1997-2000 C5 cluster to consider EBCM communications healthy and to indicate no active EBCM-related trouble conditions.

## Capture Modes

Run these captures and save logs for comparison:

1. **Cluster only baseline**: cluster powered, no emulator traffic.
2. **Cluster + passive sniffer**: verify natural request cadence and timeout behavior.
3. **Cluster + emulator candidate**: inject trial heartbeat/status frames and observe cluster response.

## Logging Requirements

For each frame, log:

- Timestamp in milliseconds
- Message ID/header bytes
- Data payload bytes
- Checksum/CRC if your tool exposes it
- Source/direction marker if available

Use CSV or line-based logs that can be diffed.

## Discovery Steps

1. Capture 60 seconds immediately after cluster power-up.
2. Capture an additional 5 minutes of steady-state traffic.
3. Note the period of repeated request frames likely targeting brake/EBCM functions.
4. Identify any request IDs that repeat at fixed intervals (for example 50 ms, 100 ms, 250 ms, 1 s).
5. Create a shortlist of candidate frames that, when unanswered, correlate with warning changes.

## Deriving Minimum Message Set

Define these candidate categories:

- **Presence/heartbeat frame(s)**: repeated periodic status from EBCM identity.
- **On-demand response frame(s)**: answers to module status/DTC queries.
- **No-active-code status frame(s)**: payload bytes that suppress active fault display.

For each category, record:

- Expected cycle time
- Allowed jitter target
- Required payload bytes and invariant fields
- Whether request-triggered or periodic broadcast

## Acceptance Criteria for Discovery

Discovery is complete when:

- You can list required periodic frame IDs and cycle times.
- You can list required request IDs and corresponding response IDs.
- You can identify payload values that represent "no active codes."
- Cluster behavior remains stable for 10+ minutes without EBCM comm warnings.

## Output Table Template

Populate this table after capture analysis:

| Type | Request ID | Response ID | Period (ms) | Payload Template | Notes |
|---|---|---:|---:|---|---|
| Presence | N/A | TBD | TBD | TBD | Periodic broadcast |
| DTC Status | TBD | TBD | on request | TBD | Must indicate no active codes |
| Health/Status | TBD | TBD | TBD | TBD | Byte-level interpretation pending |
