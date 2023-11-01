/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QWidget>
#include <QPainter>
#include <QTextBlock>
#include <QtDebug>
#include <QMenu>
#include <QAction>

#include "codeeditor.h"
#include "emc.hh"
#include "emc_nml.hh"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    act_clear = new QAction(tr("clean"),this);
    connect(act_clear,SIGNAL(triggered()),this,SLOT(clear_data()));


    menu = new MyMenu();
    menu->installEventFilter(menu);

    act_edit = new QAction(tr("edit"),this);
    connect(act_edit,SIGNAL(triggered()),this,SIGNAL(ed_edit()));

    act_flush = new QAction(tr("flush"),this);

    act_unload = new QAction(tr("unload"),this);
    connect(act_unload,SIGNAL(triggered()),this,SIGNAL(ed_unload()));


    connect(this,SIGNAL(selectionChanged()),this,SLOT(setcursornow()));
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    loading = false;

    //this->installEventFilter(this);

}


void CodeEditor::set_line_pos(int linenum)
{
    QTextCursor cur = this->textCursor();
    cur.setPosition(this->document()->findBlockByNumber(linenum).position());
    this->setTextCursor(cur);
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

bool CodeEditor::isLoading() {

    if(loading)
        return true;
    else
        return false;

}

void CodeEditor::setUnblockText(QString &text) {
    timeId = startTimer(200);
    if(timeId != 0) {
        currentStr =  text;
        loading = true;
        index = 0;
        clear();
    }

}

void CodeEditor::setcursornow() {

    //QTextCursor cursor = this->textCursor();
    //this->setTextCursor(cursor);

}

void CodeEditor::scrollContentsBy(int dx, int dy)
{
    if(loading)
       return;
    QPlainTextEdit::scrollContentsBy(dx,dy);
}

void CodeEditor::mousePressEvent(QMouseEvent * e) {
     if(!loading) {
        QPlainTextEdit::mousePressEvent(e);
     }
     return;
}

void CodeEditor::mouseDoubleClickEvent(QMouseEvent * e) {
    if(!loading) {
       QPlainTextEdit::mouseDoubleClickEvent(e);
    }
    return;
}

void CodeEditor::timerEvent(QTimerEvent *e)
{

    if(e->timerId() == timeId && loading) {

        QString tmp;
        tmp = currentStr.mid(index,500*1024);
        index += 500*1024;

        if(index < currentStr.size()) {

            bool full = false;
            while( (currentStr.at(index) != '\n') ) {
                tmp.append(currentStr.at(index));
                index++;
                if(index >= currentStr.size()) {
                    full = true;
                    break;
                }
            }
            index++;
            tmp.append('\n');
        }

        this->textCursor().insertText(tmp);

        if(tmp.isEmpty()) {
            killTimer(timeId);
            loading = false;
            QTextCursor cursor = this->textCursor();
            cursor.setPosition(0);
            this->setTextCursor(cursor);
            this->setUndoRedoEnabled(false);
            this->setUndoRedoEnabled(true);

            emit ed_load_done();

        }
    }

}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

//    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor("#BCD2EE");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
 //   }

        current_line = selection.cursor.blockNumber();
        if(emcStatus)if(emcStatus->task.interpState == EMC_TASK_INTERP_IDLE)
            emit ed_opengl_highline(current_line);
        linetext = this->document()->findBlockByLineNumber(current_line).text();
    setExtraSelections(extraSelections);
}


void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::contextMenuEvent(QContextMenuEvent *)
{
    if(menu_type == 1){

    QCursor cur = this->cursor();
    static QMenu *menu = new QMenu();
    menu->clear();
    menu->addAction(act_clear);
    menu->exec(cur.pos());

    }else if(0 == menu_type){

        QCursor cur = this->cursor();
        menu->clear();
        menu->addAction(act_flush);
        menu->addAction(act_edit);
        menu->addAction(act_unload);
        menu->exec(cur.pos());

    }


}

void CodeEditor::clear_data()
{
    emit(ed_clear_data());
}

void CodeEditor::set_run_from_line()
{
    emit ed_run_from_line(current_line);
}

bool CodeEditor::eventFilter(QObject *obj, QEvent *ev) {



    return QPlainTextEdit::eventFilter(obj,ev);
}

bool MyMenu::eventFilter(QObject *obj, QEvent *ev) {

    if(ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mev = (QMouseEvent*)ev;
        if(mev->button() == Qt::RightButton) {
                return true;
        }

    }
    if(ev->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mev = (QMouseEvent*)ev;
        if(mev->button() == Qt::RightButton) {
                return true;
        }
    }
    return QMenu::eventFilter(obj,ev);
}

MyMenu::MyMenu(QWidget *parent) : QMenu(parent) {

}
