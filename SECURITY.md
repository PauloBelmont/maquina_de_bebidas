# Security Policy

This is an embedded academic project (ESP32 + Nextion). Security matters both for firmware integrity and for physical safety (pumps/relays). Please report vulnerabilities responsibly.

## Supported Versions

Security fixes are provided only for the latest version on the default branch.

| Version/Branch | Supported |
| --- | --- |
| `main` | Yes |
| Older tags/branches | No |

## Reporting a Vulnerability

If you find a security issue, please **do not open a public issue** right away.

1. Contact the maintainers privately (email): **[ADD CONTACT EMAIL HERE]**
2. Subject: `SECURITY REPORT - ESP32 Nextion Beverage Machine`
3. Include:
   - Clear reproduction steps
   - Logs/serial output (if available)
   - What hardware setup you used (if relevant)

A response is expected within **48–72 hours**.

## Scope (Embedded / IoT / Hardware)

### In scope (examples)

- **Serial/UART injection** that can trigger pumps/relays without the intended flow (e.g., bypass payment/logic).
- **Buffer overflow / memory corruption** due to unsafe string handling.
- **Actuator fail-safe bugs** that may leave a pump/relay ON indefinitely (physical risk).

### Out of scope (examples)

- **Physical access attacks** (e.g., plugging USB and reflashing firmware) when the attacker has direct access to the device.
- **Reverse engineering** of mechanical parts or case design.

## Secure Development Guidelines

- Validate and sanitize all inputs received from the Nextion display over Serial.
- Avoid committing secrets (Wi‑Fi passwords, API keys, tokens). Use ignored config headers or local secrets.
- Prefer fail-safe defaults: on error, ensure actuators are turned OFF and the system returns to a safe state.

## Disclosure

After a fix is released, a brief security note may be published in a release/changelog, with reporter credit if approved.
