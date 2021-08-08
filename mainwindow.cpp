#include "mainwindow.h"

#include <QDebug>

CMainWindow::CMainWindow()
{
	m_resizeTimer.setSingleShot(true);
	connect(&m_resizeTimer, SIGNAL(timeout()), SLOT(resizeTimeOut()));
}

void CMainWindow::resizeEvent(QResizeEvent* revent)
{
	QQuickWindow::resizeEvent(revent);
	m_width = revent->size().width();
	m_height = revent->size().height();
	m_resizeTimer.start(m_resizeTimeOut);
}

void CMainWindow::resizeTimeOut()
{
	emit resizeEnded(m_width, m_height);
}
