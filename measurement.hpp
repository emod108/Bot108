#ifndef BOT108_MEASUREMENT_HPP
#define BOT108_MEASUREMENT_HPP

#include <functional>
#include <vector>

// To make conversion easily, one base unit of each type will be choosen, and using it we will be able to convert everything

enum MeasurementType {
    MT_LENGTH, // Basic unit - Metre
    MT_WEIGHT, // Basic unit - Kilogram
    MT_TEMPERATURE // Basic unit - Celcius
};

enum MeasurementSystem {
    MS_METRIC, // Metric measurement system
    MS_US // US standard measurement system
};

struct MeasurementUnit {
    MeasurementType type;
    std::string name;
    bool isBase;
    MeasurementSystem system;
    std::function<double(const double)> fromBase;
    std::function<double(const double)> toBase;
};

struct Measurement {
    Measurement(const std::string &unitName, const double value = 0);

    MeasurementUnit unit;
    double value;

    // convert to is the name of the unit we need to convert to
    void convert(const std::string &convertTo);
};

// This is our conversion table
// Here for every measurement unit we store:
// measurement type, name, is it base, measurement system,
// how to convert FROM base TO this, how to convert TO base FROM this
inline const std::vector<MeasurementUnit> CONVERSION_TABLE {
    /* Length */
    { MT_LENGTH, "Milimetre", false, MS_METRIC,
        [] (const double metres) { return metres * 1e3; },
        [] (const double milimetres) { return milimetres / 1e3; }
    },

    { MT_LENGTH, "Centimetre", false, MS_METRIC,
        [] (const double metres) { return metres * 1e2; },
        [] (const double centimetres) { return centimetres / 1e2; }
    },

    { MT_LENGTH, "Decimetre", false, MS_METRIC,
        [] (const double metres) { return metres * 1e1; },
        [] (const double decimetres) { return decimetres / 1e1; }
    },

    { MT_LENGTH, "Metre", true, MS_METRIC,
        [] (const double metres) { return metres; },
        [] (const double metres) { return metres; }
    },

    { MT_LENGTH, "Kilometre", false, MS_METRIC,
        [] (const double metres) { return metres * 1e-3; },
        [] (const double kilometres) { return kilometres / 1e-3; }
    },

    { MT_LENGTH, "Inch", false, MS_US,
        [] (const double metres) { return metres * 39.3700787; },
        [] (const double inches) { return inches / 39.3700787; }
    },

    { MT_LENGTH, "Foot", false, MS_US,
        [] (const double metres) { return metres * 3.2808399; },
        [] (const double feet) { return feet / 3.2808399; }
    },

    { MT_LENGTH, "Yard", false, MS_US,
        [] (const double metres) { return metres * 1.0936133; },
        [] (const double yards) { return yards / 1.0936133; }
    },

    { MT_LENGTH, "Mile", false, MS_US,
        [] (const double metres) { return metres * 0.000621371192; },
        [] (const double miles) { return miles / 0.000621371192; }
    },

    /* Weight */
    { MT_WEIGHT, "Milligram", false, MS_METRIC,
        [] (const double kilograms) { return kilograms * 1e6; },
        [] (const double milligrams) { return milligrams / 1e6; }
    },

    { MT_WEIGHT, "Gram", false, MS_METRIC,
        [] (const double kilograms) { return kilograms * 1e3; },
        [] (const double grams) { return grams / 1e3; }
    },

    { MT_WEIGHT, "Kilogram", true, MS_METRIC,
        [] (const double kilograms) { return kilograms; },
        [] (const double kilograms) { return kilograms; }
    },

    // Metric tonne. Don't confuse with US ton (short ton)
    { MT_WEIGHT, "Tonne", false, MS_METRIC, 
        [] (const double kilograms) { return kilograms * 1e-3; },
        [] (const double tonnes) { return tonnes / 1e-3; }
    },

    { MT_WEIGHT, "Ounce", false, MS_US,
        [] (const double kilograms) { return kilograms * 35.2739619; },
        [] (const double ounces) { return ounces / 35.2739619; }
    },

    { MT_WEIGHT, "Pound", false, MS_US,
        [] (const double kilograms) { return kilograms * 2.20462262; },
        [] (const double pounds) { return pounds / 2.20462262; }
    },

    // Us ton (short ton). Don't confuse with Metic tonne
    { MT_WEIGHT, "Ton", false, MS_US,
        [] (const double kilograms) { return kilograms * 0.00110231131; },
        [] (const double tons) { return tons / 0.00110231131; }
    },

    /* Temperature */
    { MT_TEMPERATURE, "Celcius", true, MS_METRIC,
        [] (const double celcius) { return celcius; },
        [] (const double celcius) { return celcius; }
    },

    { MT_TEMPERATURE, "Kelvin", false, MS_METRIC,
        [] (const double celcius) { return celcius + 273.15; },
        [] (const double kelvin) { return kelvin - 273.15; }
    },

    { MT_TEMPERATURE, "Fahrenheit", false, MS_US,
        [] (const double celcius) { return celcius * 1.8 + 32.0; },
        [] (const double fahrenheit) { return (fahrenheit - 32.0) / (5.0 / 9.0); }
    }
};

#endif // BOT108_MEASUREMENT_HPP