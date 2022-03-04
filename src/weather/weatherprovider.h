#include<iostream>
#include<cstdio>

namespace Weather {

    enum class MeasurementUnit :char
    {
        METRIC = 'm', IMPERIAL = 'i'
    };

    struct WeatherStruct {

        double temperatureLow = { 0 };
        double temperatureHigh = { 0 };
        double temperatureCurrent = { 0 };
        double humidity = { 0 };
        double pressure = { 0 };
        double windSpeed = { 0 };
        time_t dateTime = { 0 };
        MeasurementUnit unit = { MeasurementUnit::METRIC };

        std::string timestampToString() {
            auto timeinfo = localtime(&dateTime);
            return std::asctime(timeinfo);
        }
    };

};



namespace Weather {
    class ProviderAPI {

    public:

        ProviderAPI(const std::wstring  key) {
            setApiKey(key);
            setUnit();
        };
        virtual WeatherStruct getWeather(double latitude, double longitude) = 0;
        /// <summary>
        /// Set last error message
        /// </summary>
        /// <param name="message"></param>
        /// <returns></returns>
        std::string setLastError(std::wstring message) {
            this->lastErrorMessage = message;
        }
        /// <summary>
        /// Return last error message
        /// </summary>
        /// <returns></returns>
        std::wstring getLastError() {
            return this->lastErrorMessage;
        }
        /// <summary>
        /// Set Api Key
        /// </summary>
        /// <param name="key"></param>
        void setApiKey(std::wstring key) {

            this->apiKey = key;
        };
        /// <summary>
        /// Get API key
        /// </summary>
        /// <returns></returns>
        std::wstring getApiKey() {

            return this->apiKey;
        };

        /// <summary>
        /// Set measurement unit
        /// </summary>
        /// <param name="unit"></param>
        void setUnit(MeasurementUnit unit=MeasurementUnit::METRIC) {
            this->apiUnit = unit;
        };

        /// <summary>
        /// Return measurement unit 
        /// </summary>
        /// <returns></returns>
        MeasurementUnit getUnit() {
            return this->apiUnit;
        }

    private:
        std::wstring apiKey;
        std::wstring lastErrorMessage;
        MeasurementUnit apiUnit;


    };
    


};