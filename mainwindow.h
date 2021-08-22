#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QQuickWindow>
#include <QQuickItem>
#include <QTimer>

class CalculationsTest;

class CMainWindow : public QQuickWindow
{
	Q_OBJECT
public:
	CMainWindow();

public slots:
	void resizeTimeOut();
	void setCalculationsTest(CalculationsTest *calculationsTest);

signals:
	void resizeEnded(int widht, int height);

protected:
	void resizeEvent(QResizeEvent *) override;
	void keyPressEvent(QKeyEvent *) override;

protected:
	QTimer m_resizeTimer;
	static const int m_resizeTimeOut = 500;
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	CalculationsTest *m_calculationTest = nullptr;
};

#endif // CMAINWINDOW_H
