// This is the main loop where the work is done

// Read sensormodule values
if (senType == 0) {
  curAltitude = bmp280.readAltitude(1013.25) * 100; // In Centimeter
  uTemperature = bmp280.readTemperature() * 10; // In Celsius ( x10 for one decimal)
  uPressure = bmp280.readPressure(); // In Pascal (100 Pa = 1 hPa = 1 mbar)
}
if (senType == 1) {
  curAltitude = bme280.readAltitude(1013.25) * 100; // In Centimeter
  uTemperature = bme280.readTemperature() * 10; // In Celsius ( x10 for one decimal)
  uPressure = bme280.readPressure(); // In Pascal (100 Pa = 1 hPa = 1 mbar)
  uHumidity = bme280.readHumidity() * 10; // In %rH
}
if (senType == 2) {
  curAltitude = bmp085.readAltitude(1013.25) * 100; // In Centimeter
  uTemperature = bmp085.readTemperature() * 10; // In Celsius ( x10 for one decimal)
  uPressure = bmp085.readPressure(); // In Pascal (100 Pa = 1 hPa = 1 mbar)
}

// Set start-altitude in sensor-start
if (uLoopCount == 9) {
  startAltitude = curAltitude;
  lastVariofilter = 0;
}

// Vario calculation
uVario = (curAltitude - lastAltitude) * (1000 / float(millis() - lastTime));
lastTime = millis();
lastAltitude = curAltitude;

// Vario Filter
double fX;
double fY = uVario;
fX = FILTER_X * lastVariofilter;
fY = FILTER_Y * fY;
lastVariofilter = fX + fY;
uVario = lastVariofilter;

// Dead zone filter
if (uVario > DEADZONE_UP) {
  uVario -= DEADZONE_UP;
}
else if (uVario < DEADZONE_DOWN) {
  uVario -= DEADZONE_DOWN;
}
else {
  uVario = 0;
}

// Altitude
uAltitude = (curAltitude - startAltitude) / 10;
uAbsAltitude = curAltitude / 100;

if (uLoopCount < 10) {
  uLoopCount++;
  uVario = 0;
  uAltitude = 0;
  uAbsAltitude = 0;
}

// EU to US conversions
// ft/s = m/s / 0.3048
// inHG = hPa * 0,029529983071445
// ft = m / 0.3048
if (units == 1) {
  uAltitude = uAltitude / 0.3048;
  uAbsAltitude = uAbsAltitude / 0.3048;
  uVario = uVario / 0.3048;
  uPressure = uPressure * 0.029529983071445;
  uTemperature = uTemperature * 1.8 + 32;
}