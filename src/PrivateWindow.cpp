#include "PrivateWindow.h"
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineCookieStore>
#include <QAction>
#include <QToolButton>
#include <QProgressBar>
#include <QStatusBar>
#include <QCloseEvent>
#include <QUrl>
#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QSizePolicy>
#include <QFile>
// ── Theme colors ──────────────────────────────────────────────────────────────
static const QString BG_DARK    = "#0a0a0a";
static const QString BG_TOOLBAR = "#111111";
static const QString BG_TAB     = "#151515";
static const QString ACCENT     = "#00ff88";
static const QString ACCENT_DIM = "#00cc66";
static const QString TEXT_MAIN  = "#e8e8e8";
static const QString TEXT_DIM   = "#666666";
static const QString BORDER     = "#1e1e1e";

PrivateWindow::PrivateWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Nothing Private Browser");

    QPixmap iconPixmap(":/icons/logomain.png");
    if (!iconPixmap.isNull())
        setWindowIcon(QIcon(iconPixmap));

    resize(1280, 800);

    m_profile = new QWebEngineProfile(this);
    m_interceptor = new Interceptor(this);
    m_profile->setUrlRequestInterceptor(m_interceptor);

    QWebEngineScript fpScript;
    fpScript.setName("NPB_FP");
    fpScript.setSourceCode(FingerprintSpoofer::instance().injectionScript());
    fpScript.setInjectionPoint(QWebEngineScript::DocumentCreation);
    fpScript.setWorldId(QWebEngineScript::MainWorld);
    fpScript.setRunsOnSubFrames(true);
    m_profile->scripts()->insert(fpScript);

    m_tabs = new QTabWidget(this);
    m_tabs->setTabsClosable(true);
    m_tabs->setMovable(true);
    m_tabs->setDocumentMode(true);

    // Logo on the tab bar corner
    QLabel *logoLabel = new QLabel();
    QPixmap logo(":/icons/logomain.png");
    if (!logo.isNull()) {
        logoLabel->setPixmap(logo.scaledToHeight(28, Qt::SmoothTransformation));
    } else {
        logoLabel->setText("⚡");
        logoLabel->setStyleSheet("color: #00ff88; font-size: 20px;");
    }
    logoLabel->setContentsMargins(8, 0, 8, 0);
    m_tabs->setCornerWidget(logoLabel, Qt::TopLeftCorner);

    connect(m_tabs, &QTabWidget::tabCloseRequested, this, &PrivateWindow::closeTab);

    setCentralWidget(m_tabs);
    setupToolbar();
    applyTheme();

    m_updater = new UpdateChecker(this);

    newTab(QUrl("about:blank"));
}

PrivateWindow::~PrivateWindow() { purge(); }

// ── Theme ─────────────────────────────────────────────────────────────────────
void PrivateWindow::applyTheme() {
    qApp->setStyle("Fusion");

    setStyleSheet(QString(R"(
        QMainWindow {
            background: %1;
        }
        QToolBar {
            background: %2;
            border-bottom: 1px solid %6;
            padding: 4px 8px;
            spacing: 4px;
        }
        QToolBar QToolButton {
            background: transparent;
            color: %4;
            border: none;
            border-radius: 4px;
            padding: 4px 8px;
            font-size: 16px;
        }
        QToolBar QToolButton:hover {
            background: %7;
            color: %5;
        }
        QLineEdit {
            background: #1a1a1a;
            color: %3;
            border: 1px solid %6;
            border-radius: 20px;
            padding: 6px 16px;
            font-size: 13px;
            selection-background-color: %4;
        }
        QLineEdit:focus {
            border: 1px solid %4;
        }
        QTabWidget::pane {
            border: none;
            background: %1;
        }
        QTabBar {
            background: %2;
        }
        QTabBar::tab {
            background: %7;
            color: %8;
            border: none;
            padding: 8px 16px;
            margin-right: 2px;
            border-radius: 4px 4px 0 0;
            min-width: 120px;
            max-width: 200px;
            font-size: 12px;
        }
        QTabBar::tab:selected {
            background: %1;
            color: %3;
            border-top: 2px solid %4;
        }
        QTabBar::tab:hover {
            background: #1e1e1e;
            color: %3;
        }
        QTabBar::close-button {
            image: none;
            subcontrol-position: right;
        }
        QStatusBar {
            background: %2;
            color: %8;
            font-size: 11px;
            border-top: 1px solid %6;
        }
        QProgressBar {
            background: transparent;
            border: none;
            height: 2px;
        }
        QProgressBar::chunk {
            background: %4;
            border-radius: 1px;
        }
    )")
    .arg(BG_DARK)       // 1
    .arg(BG_TOOLBAR)    // 2
    .arg(TEXT_MAIN)     // 3
    .arg(ACCENT)        // 4
    .arg(ACCENT_DIM)    // 5
    .arg(BORDER)        // 6
    .arg(BG_TAB)        // 7
    .arg(TEXT_DIM)      // 8
    );
}

// ── Toolbar ───────────────────────────────────────────────────────────────────
void PrivateWindow::setupToolbar() {
    m_toolbar = addToolBar("Navigation");
    m_toolbar->setMovable(false);
    m_toolbar->setIconSize(QSize(16, 16));

    QAction *backAct   = m_toolbar->addAction("◀");
    QAction *fwdAct    = m_toolbar->addAction("▶");
    QAction *reloadAct = m_toolbar->addAction("↺");
    m_toolbar->addSeparator();

    m_loadingDots = new QLabel();
    m_loadingDots->setText("...");
    m_loadingDots->setStyleSheet("color: #00ff88; font-size: 16px;");
    m_loadingDots->setFixedSize(24, 24);
    m_loadingDots->setVisible(false);
    m_toolbar->addWidget(m_loadingDots);

    m_urlBar = new QLineEdit(this);
    m_urlBar->setPlaceholderText("  Search or enter address...");
    m_urlBar->setClearButtonEnabled(true);
    m_urlBar->setMinimumWidth(400);
    m_urlBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_toolbar->addWidget(m_urlBar);

    m_progress = new QProgressBar(this);
    m_progress->setFixedHeight(2);
    m_progress->setRange(0, 100);
    m_progress->setTextVisible(false);
    m_progress->setVisible(false);
    statusBar()->addWidget(m_progress, 1);

    m_toolbar->addSeparator();
    QAction *newTabAct = m_toolbar->addAction("＋  New Tab");

    connect(backAct,   &QAction::triggered, this, [this]() { if (auto *v = currentView()) v->back(); });
    connect(fwdAct,    &QAction::triggered, this, [this]() { if (auto *v = currentView()) v->forward(); });
    connect(reloadAct, &QAction::triggered, this, [this]() { if (auto *v = currentView()) v->reload(); });
    connect(newTabAct, &QAction::triggered, this, [this]() { newTab(QUrl("about:blank")); });
    connect(m_urlBar,  &QLineEdit::returnPressed, this, [this]() {
        navigateTo(m_urlBar->text().trimmed());
    });
}

// ── Tab management ────────────────────────────────────────────────────────────
void PrivateWindow::newTab(const QUrl &url) {
    auto *view = new QWebEngineView(this);
    auto *page = new QWebEnginePage(m_profile, view);
    view->setPage(page);

    if (url == QUrl("about:blank")) {
        int index = m_tabs->addTab(view, "  Home");
        m_tabs->setCurrentIndex(index);

        QFile f(":/home.html");
        f.open(QIODevice::ReadOnly);
        QString html = f.readAll();
        view->setHtml(html, QUrl("qrc:///"));
        return;
    }

    int index = m_tabs->addTab(view, "  Loading...");
    m_tabs->setCurrentIndex(index);

    connect(view, &QWebEngineView::titleChanged, this, [this, view](const QString &t) {
        int i = m_tabs->indexOf(view);
        if (i >= 0) m_tabs->setTabText(i, "  " + (t.isEmpty() ? "New Tab" : t.left(28)));
    });
    connect(view, &QWebEngineView::urlChanged, this, [this, view](const QUrl &u) {
        if (m_tabs->currentWidget() == view)
            m_urlBar->setText(u.toString());
    });
    connect(view, &QWebEngineView::loadStarted, this, [this, view]() {
        if (m_tabs->currentWidget() == view) {
            m_loadingDots->setVisible(true);
            m_progress->setVisible(true);
            m_progress->setValue(10);
        }
    });
    connect(view, &QWebEngineView::loadProgress, this, [this, view](int p) {
        if (m_tabs->currentWidget() == view)
            m_progress->setValue(p);
    });
    connect(view, &QWebEngineView::loadFinished, this, [this, view](bool ok) {
        if (m_tabs->currentWidget() == view) {
            m_loadingDots->setVisible(false);
            m_progress->setVisible(false);
            statusBar()->showMessage(ok ? "" : "Failed to load", 2000);
        }
    });
    connect(m_tabs, &QTabWidget::currentChanged, this, [this](int) {
        if (auto *v = currentView())
            m_urlBar->setText(v->url().toString());
        else
            m_urlBar->clear();
    });

    view->load(url);
}

void PrivateWindow::closeTab(int index) {
    if (m_tabs->count() == 1) { close(); return; }
    auto *widget = m_tabs->widget(index);
    m_tabs->removeTab(index);
    widget->deleteLater();
}

// ── Navigation ────────────────────────────────────────────────────────────────
void PrivateWindow::navigateTo(const QString &input) {
    if (!currentView()) {
        newTab(resolveInput(input));
        return;
    }
    currentView()->load(resolveInput(input));
}

QUrl PrivateWindow::resolveInput(const QString &input) const {
    if (input.startsWith("http://") || input.startsWith("https://"))
        return QUrl(input);
    if (input.contains(".") && !input.contains(" "))
        return QUrl("https://" + input);
    return QUrl("https://www.google.com/search?q=" + QUrl::toPercentEncoding(input));
}

// ── Helpers ───────────────────────────────────────────────────────────────────
QWebEngineView *PrivateWindow::currentView() const {
    return qobject_cast<QWebEngineView *>(m_tabs->currentWidget());
}

void PrivateWindow::purge() {
    if (!m_profile) return;
    m_profile->clearHttpCache();
    m_profile->clearAllVisitedLinks();
    m_profile->cookieStore()->deleteAllCookies();
}

void PrivateWindow::closeEvent(QCloseEvent *event) {
    purge();
    event->accept();
}

bool PrivateWindow::eventFilter(QObject *obj, QEvent *event) {
    return QMainWindow::eventFilter(obj, event);
}

void PrivateWindow::onUrlChanged(const QUrl &url) { Q_UNUSED(url); }
void PrivateWindow::onLoadStarted() {}
void PrivateWindow::onLoadFinished(bool ok) { Q_UNUSED(ok); }