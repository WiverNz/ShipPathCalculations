#pragma once

#include "common/CryCommon.h"
#include "common/NavigationTypes.h"

//! \namespace Calculations
//! \brief The calculations namespace. Here only dependence with the Common is possible
namespace Calculations
{
//! \struct IMovementCalculator
//! \brief Interface for {CMovementCalculator}
struct IMovementCalculator
{
	virtual void SetFrameTime(const float frameTime) = 0;
	virtual void Calculate() const = 0;
	virtual void AccelerationCalculation() const = 0;
	virtual Vec3 GetNextPos(const Vec3& currPos) const = 0;
	virtual Vec3 GetNextDiffPos(const Vec3& currPos, const float& frameTime) const = 0;
};

DECLARE_SHARED_POINTERS(IMovementCalculator);

//! \class CMovementCalculator
//! \brief This is where all magic has to happen.
class CMovementCalculator : public IMovementCalculator
{
	CRYINTERFACE_SIMPLE(IMovementCalculator)
public:
	explicit CMovementCalculator(const float frameTime, const Navigation::SShipParameters* shipParameters, Navigation::SStatusShip* statusShip);
	virtual ~CMovementCalculator() = default;

	void SetFrameTime(const float frameTime) override;
	void Calculate() const override;
	Vec3 GetNextPos(const Vec3& currPos) const override;
	Vec3 GetNextDiffPos(const Vec3& currPos, const float& frameTime) const override;
	void AccelerationCalculation() const override;

protected:
	float m_frameTime = 0;
	const Navigation::SShipParameters* m_shipParameters = nullptr;
	Navigation::SStatusShip* m_statusShip = nullptr;
};

} // namespace Calculations
