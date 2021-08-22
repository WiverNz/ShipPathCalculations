#include "mainwindow.h"
#include "CalculationsTest/CalculationsTest.h"

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

void CMainWindow::keyPressEvent(QKeyEvent* revent)
{
	QQuickWindow::keyPressEvent(revent);
	if (m_calculationTest) {
		m_calculationTest->keyPressEvent(revent);
	}
}

void CMainWindow::resizeTimeOut()
{
	emit resizeEnded(m_width, m_height);
}

void CMainWindow::setCalculationsTest(CalculationsTest *calculationsTest)
{
	m_calculationTest = calculationsTest;
}
