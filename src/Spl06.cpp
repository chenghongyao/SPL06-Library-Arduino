#include "Spl06.h"

using namespace dps;
using namespace dps310;

void Spl06::init(void)
{
	int16_t prodId = readByteBitfield(registers[PROD_ID]);
	if (prodId < 0)
	{
		//Connected device is not a Dps310
		m_initFail = 1U;
		return;
	}
	m_productID = prodId;

	int16_t revId = readByteBitfield(registers[REV_ID]);
	if (revId < 0)
	{
		m_initFail = 1U;
		return;
	}
	m_revisionID = revId;

	//find out which temperature sensor is calibrated with coefficients...
	// int16_t sensor = readByteBitfield(registers[TEMP_SENSORREC]);
	// if (sensor < 0)
	// {
	// 	m_initFail = 1U;
	// 	return;
	// }
	int16_t sensor = 1;

	//...and use this sensor for temperature measurement
	m_tempSensor = sensor;
	if (writeByteBitfield((uint8_t)sensor, registers[TEMP_SENSOR]) < 0)
	{
		m_initFail = 1U;
		return;
	}

	//read coefficients
	if (readcoeffs() < 0)
	{
		m_initFail = 1U;
		return;
	}

	//set to standby for further configuration
	standby();

	//set measurement precision and rate to standard values;
	configTemp(DPS__MEASUREMENT_RATE_4, DPS__OVERSAMPLING_RATE_8);
	configPressure(DPS__MEASUREMENT_RATE_4, DPS__OVERSAMPLING_RATE_8);

	//perform a first temperature measurement
	//the most recent temperature will be saved internally
	//and used for compensation when calculating pressure
	float trash;
	measureTempOnce(trash);

	//make sure the DPS310 is in standby after initialization
	standby();

	// Fix IC with a fuse bit problem, which lead to a wrong temperature
	// Should not affect ICs without this problem
	correctTemp();
}