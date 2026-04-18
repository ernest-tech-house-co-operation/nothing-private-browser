## Contributing to Nothing Browser

Thanks for your interest! Before you start, please read our [**full Contributing Guide**](https://nothing-browser-docs.pages.dev/guide/community/contributing) for complete details.

**Here’s the short version:**

### ⚠️ Do Not Touch the Identity Core
PRs modifying fingerprint spoofing, identity generation, or identity store files (like `FingerprintSpoofer.cpp`, `IdentityGenerator.cpp`) will be **rejected without review**. If you suspect a bug, please open an issue first.

### ✅ What We Welcome
*   Bug fixes
*   New UI features or tabs
*   Build and CI improvements
*   Documentation updates
*   Performance improvements (outside the identity core)

### 🔧 Building Locally
Basic steps (see full guide for details):
```bash
# Install Qt6 dependencies (Debian/Ubuntu example)
sudo apt-get install qt6-base-dev qt6-webengine-dev libqt6webenginewidgets6 cmake ninja-build build-essential

# Build the NewPipe JAR bridge
cd newpipe-bridge && ./gradlew shadowJar --no-daemon && cd ..

# Configure and build the browser
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build --parallel --target nothing-browser nothing-browser-headless nothing-browser-headful
```

### 📝 Submitting a PR
*   Keep it focused (one change per PR)
*   Ensure the build succeeds before opening the PR
*   Fill out the PR template
*   Be patient – this is a small team

### ❓ Questions?
Open an issue on GitHub or email **ernesttechhouse@gmail.com**

---

**For the complete guide, including rules for the Private Browser, Piggy library, and creating community language libraries, please visit:**  
👉 [https://nothing-browser-docs.pages.dev/guide/community/contributing](https://nothing-browser-docs.pages.dev/guide/community/contributing)