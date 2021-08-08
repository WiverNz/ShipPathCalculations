#ifndef CALCULATIONSTEST_H
#define CALCULATIONSTEST_H

#include <QQuickItem>
#include <QImage>
#include <QQuickPaintedItem>
#include <QMetaType>
#include <QMutex>

#include "Calculations/MovementCalculator.h"

class CalculationsTest;

enum class ETest
{
	Speed_1x1,
	Speed_1x10,
	Speed_1x60,
	Speed_x10,
	LastTest
};

class ICalculationTest
{
public:
	using Positions = QVector<Vec3>;
	using ConstPositions = const Positions&;

	ICalculationTest(const CalculationsTest* calculations, const ETest& test);
	virtual ~ICalculationTest() = default;
	virtual void paint(QPainter* painter);
	virtual void paintInfoText(QPainter* painter, int& heightPosition, const QString& caption, const QString& value) const;
	virtual void timerUpdate(const double& diffMsTime, const double& currTime);
	virtual void resizeEnded(const int widht, const int height);
	virtual Vec3 pointToMap(const Vec3& point);
	virtual Vec3 mapToPoint(const Vec3& point);
	ConstPositions getPositions() const;
	virtual void clearPositions();
	virtual double gridCellSize() const;


protected:
	const CalculationsTest* m_calculations;
	ETest m_test = ETest::Speed_1x1;
	QImage m_map;
	QImage m_scaledMap;
	double m_mapMilesWidth = 0;
	double m_mapMilesHeight = 0;
	double m_mapGridCellSize = 1;
	QString m_name;
	QColor m_color;

	Calculations::IMovementCalculatorPtr m_moveCalc = nullptr;
	float m_frameTime = 10;
	Navigation::SShipParameters m_shipParameters;
	Navigation::SStatusShip m_statusShip;
	Vec3 m_startPos;
	Vec3 m_currPos;
	Positions m_positions;
};

class CalculationTest : public ICalculationTest
{
public:
	CalculationTest(const CalculationsTest* calculations, const ETest& test);
};

class CalculationsTest : public QQuickPaintedItem
{
	using ICalculationTestPtr = std::shared_ptr<ICalculationTest>;
	Q_OBJECT
	Q_PROPERTY(QVariantList positions READ positions NOTIFY positionsChanged)
	Q_PROPERTY(double gridCellSize READ gridCellSize WRITE setGridCellSize)
public:
	explicit CalculationsTest(QQuickItem *parent = nullptr);
	Q_INVOKABLE void clearCalculation();
	Q_INVOKABLE void timerUpdate(const double& diffMsTime, const double& currTime);
	Q_INVOKABLE void resizeEnded(const int widht, const int height);
	void updatePositions();
	void paint(QPainter* painter) override;

	void prevTest();
	void nextTest();

	const QVariantList& positions() const;
	double gridCellSize() const;
	void setGridCellSize(const double& cellSize);

protected:
	ICalculationTestPtr m_calculationTest;
	QVariantList m_positions;
	double m_gridCellSize = 50;
	QMutex m_mutex;

signals:
	void calculationCleared();
	void positionsChanged();

public slots:

};

#endif // CALCULATIONSTEST_H
