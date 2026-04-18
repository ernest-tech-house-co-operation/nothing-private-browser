# Nothing Private Browser

> **Does nothing... except everything that matters.**

Part of the [Nothing Browser](https://github.com/BunElysiaReact/nothing-browser) family.

A privacy-first browser built on **Qt6 + Chromium WebEngine**. No telemetry, no session persistence, no black boxes — just a browser that actually respects you.

---

## 📚 Documentation

**[https://nothing-browser-docs.pages.dev/guide/private-browser/](https://nothing-browser-docs.pages.dev/guide/private-browser/)**

Full documentation including privacy features, installation guides, and roadmap.

---

## Install

### Via apt (Recommended)

```sh
curl -fsSL https://pub-5119122a931748c3b649ad4ca5aab522.r2.dev/nothing-browser-key.gpg \
  | sudo gpg --dearmor -o /usr/share/keyrings/nothing-browser.gpg

echo 'deb [signed-by=/usr/share/keyrings/nothing-browser.gpg] https://pub-5119122a931748c3b649ad4ca5aab522.r2.dev stable main' \
  | sudo tee /etc/apt/sources.list.d/nothing-browser.list

sudo apt update && sudo apt install nothing-private-browser
```

### Linux (.deb)

```bash
sudo apt install ./nothing-private-browser_*_amd64.deb
```

### Linux (tar.gz)

```bash
tar -xzf nothing-private-browser-*-linux-x86_64.tar.gz
cd nothing-private-browser-*-linux-x86_64
./nothing-private-browser
```

### Arch Linux

```bash
yay -S nothing-private-browser
```

### macOS

Download the `.dmg` from [Releases](https://github.com/ernest-tech-house-co-operation/nothing-private-browser/releases) → drag to Applications.

### Windows

Download the `.zip` from [Releases](https://github.com/ernest-tech-house-co-operation/nothing-private-browser/releases) → extract → run `nothing-private-browser.exe`.

---

## What makes it private

- **Fingerprint spoofing** — randomised Chrome UA, hardware concurrency, device memory, screen resolution, WebGL vendor and renderer injected at engine level. Uses an xorshift PRNG for canvas noise instead of the detectable sin-based approach — because we actually read the CreepJS source
- **Zero session persistence** — cookies, cache, and storage are wiped on close. Nothing survives the session
- **Zero telemetry** — no analytics, no phoning home, nothing
- **WebRTC leak protection** — STUN servers stripped from ICE config so your real IP stays yours
- **UA-CH spoofing** — `navigator.userAgentData` and `getHighEntropyValues()` return consistent spoofed values so modern fingerprint sites don't catch the mismatch
- **Multi-tab** — full tabbed browsing, each tab isolated within the same private profile

---

## Planned

- **Ad blocker** — network-level, filter-list based
- **Tor routing** — optional onion routing, one toggle
- **ProtonVPN support** — import a `.ovpn` or WireGuard conf file directly, no separate client needed

---

## Known limitations

- Google, Facebook, and banking sites will block you — expected
- Chrome extensions not supported
- Fingerprint spoofing reduces entropy, it does not make you invisible
- Auto-update requires the binary to be writable — tar.gz builds work silently, deb installs use `pkexec` for a GUI prompt

---

## Image assets notice

Some image assets used in this project are sourced externally and may be subject to third-party rights. If a DMCA or copyright complaint is filed against any asset it will be removed promptly. Assets are not guaranteed to stay in the repo permanently — if you depend on specific images in a fork, host your own copies.

---

## License

MIT

---

*Built by [Ernest Tech House](https://github.com/ernest-tech-house-co-operation)*
