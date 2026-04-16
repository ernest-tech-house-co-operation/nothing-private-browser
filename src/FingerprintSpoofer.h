#pragma once
#include "IdentityGenerator.h"
#include <QString>

class FingerprintSpoofer {
public:
    static FingerprintSpoofer &instance() {
        static FingerprintSpoofer s;
        return s;
    }
    const BrowserIdentity &identity() const { return m_id; }
    QString injectionScript() const;
    void loadIdentity(const BrowserIdentity &id) { m_id = id; }
    void resetIdentity() { m_id = IdentityGenerator::generate(); }

private:
    FingerprintSpoofer() { m_id = IdentityGenerator::generate(); } // generate fresh, never load from disk
    BrowserIdentity m_id;
};