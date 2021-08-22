#include "CalculationsTest.h"
#include <Calculations/MovementCalculator.h>

#include <QPainter>
#include <QDebug>

ICalculationTest::ICalculationTest(const CalculationsTest* calculations, const ETest& test)
	: m_calculations(calculations), m_test(test)
{
	m_moveCalc.reset(new Calculations::CMovementCalculator(m_frameTime, &m_shipParameters, &m_statusShip));
}

void ICalculationTest::paint(QPainter* painter)
{
	if (!m_scaledMap.isNull())
	{
		painter->drawImage(0, 0, m_scaledMap);
	}
	QBrush brush(QColor("#007430"));

	painter->setBrush(brush);
	painter->setPen(Qt::NoPen);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(Qt::GlobalColor::red, 1, Qt::SolidLine));

	int textHeightPosition = 20;
	paintInfoText(painter, textHeightPosition, "Course", QString::number(m_statusShip.course.ToDegrees()));
	paintInfoText(painter, textHeightPosition, "Rudder", QString::number(m_statusShip.rudder));
	paintInfoText(painter, textHeightPosition, "Steer", QString::number(m_statusShip.steer));
	paintInfoText(painter, textHeightPosition, "Engine RPM", QString::number(m_statusShip.rightEngineRPM));
	paintInfoText(painter, textHeightPosition, "Speed (knots)", QString::number(m_statusShip.speed));
	paintInfoText(painter, textHeightPosition, "Speed", "1x");

	// QSizeF itemSize = size();
	// painter->drawRoundedRect(0, 0, itemSize.width(), itemSize.height() - 10, 10, 10);
	painter->setPen(QPen(QColor("#ff7430"), 6, Qt::SolidLine));

}

void ICalculationTest::paintInfoText(QPainter *painter, int &heightPosition, const QString &caption, const QString &value) const
{
	painter->drawText(10, heightPosition += 20, QString("%1: %2").arg(caption, value));
}

void ICalculationTest::timerUpdate(const double& diffMsTime, const double& currTime)
{
	m_moveCalc->Calculate(diffMsTime);
	m_currPos += m_moveCalc->GetNextDiffPos(m_currPos);
	m_positions.append(m_currPos);
}

void ICalculationTest::resizeEnded(const int widht, const int height)
{
	if (!m_map.isNull())
	{
		m_scaledMap = m_map.scaled(m_calculations->width(), m_calculations->height());
	}
	else
	{
		m_scaledMap = decltype(m_scaledMap)();
	}
}

Vec3 ICalculationTest::pointToMap(const Vec3& point)
{
	if (m_scaledMap.isNull())
	{
		return point;
	}

	Vec3 position;

	position.x = m_scaledMap.width() * point.x / m_mapMilesWidth;
	position.y = m_scaledMap.height() - m_scaledMap.height() * point.y / m_mapMilesHeight;
	position.z = point.z;

	return position;
}

Vec3 ICalculationTest::mapToPoint(const Vec3& point)
{
	if (m_scaledMap.isNull())
	{
		return point;
	}

	Vec3 position;

	position.x = point.x * m_mapMilesWidth / m_scaledMap.width();
	position.y = (m_scaledMap.height() - point.y) * m_mapMilesHeight / m_scaledMap.height();
	position.z = point.z;

	return position;
}

ICalculationTest::ConstPositions ICalculationTest::getPositions() const
{
	return m_positions;
}

void ICalculationTest::clearPositions()
{
	m_positions.clear();
}

double ICalculationTest::gridCellSize() const
{
	if (!m_scaledMap.isNull())
	{
		return m_mapGridCellSize / m_mapMilesWidth * m_scaledMap.width();
	}

	return 100;
}

void ICalculationTest::keyPressEvent(QKeyEvent *keyEvent)
{
	qDebug() << keyEvent;
	if (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_A)
	{
		m_moveCalc->LeftClickHandler();
	}
	else if (keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_D)
	{
		m_moveCalc->RightClickHandler();
	}
	else if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_W)
	{
		m_moveCalc->ForwardClickHandler();
	}
	else if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_S)
	{
		m_moveCalc->BackClickHandler();
	}
}

CalculationsTest::CalculationsTest(QQuickItem* parent) : QQuickPaintedItem(parent)
{
	m_calculationTest = std::make_shared<CalculationTest>(this, ETest::Speed_1x1);
}

CalculationTest::CalculationTest(const CalculationsTest *calculations, const ETest& test)
	: ICalculationTest(calculations, test)
{
	// Map settings:
	m_map.load(":/Resources/map_100ft.png");
	m_mapMilesWidth = 0.275378;
	m_mapMilesHeight = 0.167927;
	m_mapGridCellSize = 0.015;

	m_startPos = Vec3(0.059f, 0.0545f, 0.0f);
	m_currPos = m_startPos;
	m_positions.append(m_currPos);

	// Ship settings:
	m_shipParameters.engineSpeed = 375;
	m_shipParameters.maxSpeed = 7.6f;

	m_statusShip.speed = m_shipParameters.maxSpeed;
	m_statusShip.leftEngineRPM = m_shipParameters.engineSpeed;
	m_statusShip.rightEngineRPM = m_shipParameters.engineSpeed;
	m_statusShip.rudder = 0.f;
	m_statusShip.steer = 0.f;
	m_statusShip.course = CryTransform::CAngle::FromDegrees(68.5f);
}

void CalculationsTest::paint(QPainter* painter)
{
	if (m_calculationTest)
	{
		m_calculationTest->paint(painter);
	}
}

void CalculationsTest::prevTest()
{

}

void CalculationsTest::nextTest()
{

}

const QVariantList& CalculationsTest::positions() const
{
	return m_positions;
}

double CalculationsTest::gridCellSize() const
{
	return m_gridCellSize;
}

void CalculationsTest::setGridCellSize(const double& cellSize)
{
	m_gridCellSize = cellSize;
}

void CalculationsTest::keyPressEvent(QKeyEvent* keyEvent)
{
	if (m_calculationTest) {
		m_calculationTest->keyPressEvent(keyEvent);
	}
}

void CalculationsTest::clearCalculation()
{
	QMutexLocker lock(&m_mutex);
	m_calculationTest->clearPositions();
	m_positions.clear();
	emit calculationCleared();
}

void CalculationsTest::timerUpdate(const double& diffMsTime, const double& currTime)
{
	if (m_calculationTest)
	{
		m_calculationTest->timerUpdate(diffMsTime, currTime);
	}
	updatePositions();
	update();
}

void CalculationsTest::resizeEnded(const int widht, const int height)
{
	if (m_calculationTest)
	{
		m_calculationTest->resizeEnded(widht, height);
	}
	setGridCellSize(m_calculationTest->gridCellSize());
	updatePositions();
}

void CalculationsTest::updatePositions()
{
	QMutexLocker lock(&m_mutex);
	m_positions.clear();
	if (m_calculationTest)
	{
		for (const auto& point : m_calculationTest->getPositions())
		{
			const auto position = m_calculationTest->pointToMap(point);
			m_positions.append(QPoint(position.x, position.y));
		}
	}

	emit positionsChanged();
}
