#include "StdAfx.h"
#include "MovementCalculator.h"

#include <CryCore/Assert/CryAssert.h>
#include <CryMath/Angle.h>

namespace Calculations
{
CMovementCalculator::CMovementCalculator(const float frameTime, const Navigation::SShipParameters* shipParameters, Navigation::SStatusShip* statusShip)
{
	m_frameTime = frameTime;
	m_shipParameters = shipParameters;
	m_statusShip = statusShip;
}

void CMovementCalculator::SetFrameTime(const float frameTime)
{
	m_frameTime = frameTime;
}

void CMovementCalculator::Calculate() const
{
	AccelerationCalculation();
}

void CMovementCalculator::AccelerationCalculation() const
{

}

Vec3 CMovementCalculator::GetNextPos(const Vec3& currPos) const
{
	if (!m_shipParameters || !m_statusShip)
	{
		return currPos;
	}

	const float lenght = m_statusShip->speed * m_frameTime / 1000.f / 3600.f;
	auto currX = currPos.x + crymath::sin(m_statusShip->course.ToRadians()) * lenght;
	auto currY = currPos.y + crymath::cos(m_statusShip->course.ToRadians()) * lenght;

	return Vec3(currX, currY, currPos.z);
}

Vec3 CMovementCalculator::GetNextDiffPos(const Vec3& currPos, const float& frameTime) const
{
	if (!m_shipParameters || !m_statusShip)
	{
		return currPos;
	}

	const float lenght = m_statusShip->speed * frameTime / 1000.f / 3600.f;
	auto currX = crymath::sin(m_statusShip->course.ToRadians()) * lenght;
	auto currY = crymath::cos(m_statusShip->course.ToRadians()) * lenght;

	return Vec3(currX, currY, 0);
}

} // namespace Calculations
