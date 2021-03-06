#ifndef DISASSEMBLYWIDGET_H
#define DISASSEMBLYWIDGET_H

#include "Cutter.h"
#include "CutterDockWidget.h"
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QShortcut>


class DisassemblyTextEdit;
class DisassemblyScrollArea;
class DisassemblyContextMenu;

class DisassemblyWidget : public CutterDockWidget
{
    Q_OBJECT
public:
    explicit DisassemblyWidget(MainWindow *main, QAction *action = nullptr);
    QWidget *getTextWidget();

public slots:
    void highlightCurrentLine();
    void showDisasContextMenu(const QPoint &pt);
    void refreshDisasm(RVA offset = RVA_INVALID);
    void fontsUpdatedSlot();
    void colorsUpdatedSlot();
    void seekPrev();

private slots:
    void on_seekChanged(RVA offset);
    void raisePrioritizedMemoryWidget(CutterCore::MemoryWidgetType type);

    void scrollInstructions(int count);
    bool updateMaxLines();

    void cursorPositionChanged();

private:
    DisassemblyContextMenu *mCtxMenu;
    DisassemblyScrollArea *mDisasScrollArea;
    DisassemblyTextEdit *mDisasTextEdit;

    RVA topOffset;
    RVA bottomOffset;
    int maxLines;

    /*!
     * offset of lines below the first line of the current seek
     */
    int cursorLineOffset;
    bool seekFromCursor;

    RVA readCurrentDisassemblyOffset();
    RVA readDisassemblyOffset(QTextCursor tc);
    bool eventFilter(QObject *obj, QEvent *event);

    void setupFonts();
    void setupColors();

    void updateCursorPosition();

    void connectCursorPositionChanged(bool disconnect);

    void moveCursorRelative(bool up, bool page);
};

class DisassemblyScrollArea : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit DisassemblyScrollArea(QWidget *parent = nullptr);

signals:
    void scrollLines(int lines);
    void disassemblyResized();

protected:
    bool viewportEvent(QEvent *event) override;

private:
    void resetScrollBars();
};


class DisassemblyTextEdit: public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit DisassemblyTextEdit(QWidget *parent = nullptr)
        : QPlainTextEdit(parent),
          lockScroll(false) {}

    void setLockScroll(bool lock)
    {
        this->lockScroll = lock;
    }

protected:
    bool viewportEvent(QEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool lockScroll;
};

#endif // DISASSEMBLYWIDGET_H
