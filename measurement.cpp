#include "measurement.hpp"

Measurement::Measurement(const std::string &unitName, const double value)
    : value(value)
{
    // To check for errors
    bool foundUnit = false;

    // Checking for every Unit until we find our one
    for (const auto &tableUnit : CONVERSION_TABLE) {
        if (unitName == tableUnit.name) {
            unit = tableUnit;
            foundUnit = true;
            break;
        }
    }

    // This line of code should never be reached
    if (!foundUnit)
        unit = CONVERSION_TABLE[0];
}

void Measurement::convert(const std::string &convertTo)
{
    // Looking trough the table to find out unit we want to convert to
    for (const auto &tableUnit : CONVERSION_TABLE) {
        if (convertTo == tableUnit.name) {
            // Conversion supposed to happen only between same measurement types
            if (unit.type != tableUnit.type)
                return;

            // If our current measurement unit is base
            if (unit.isBase) {
                value = tableUnit.fromBase(value);
            } // If unit we want to convert to is base
            else if (tableUnit.isBase) {
                value = unit.toBase(value);
            } // If neither of our units are base
            else {
                value = unit.toBase(value); // Firstly, convert out current unit to base
                value = tableUnit.fromBase(value); // Secondly, convert our unit from base
            }

            // Change our measurement unit after conversion
            unit = tableUnit;

            // No need to stay in the loop for any longer
            return;
        }
    }
}