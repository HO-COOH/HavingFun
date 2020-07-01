/**Description: A simple C++ program to get latest Covid-19 cases from a https API
 * !Note: You need to install [curl] library to compile
 * 
 * vcpkg install curl
 */
#include <iostream>
#include <cstring>
#include <curl/curl.h>

size_t writeData(void *buffer, size_t size, size_t nmemb, void *userp)
{
    auto token = strtok(static_cast<char *>(buffer), ",");
    while (token != nullptr)
    {
        std::cout << token << '\n';
        token = strtok(nullptr, ",");
    }
    return size;
}

int main()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    if (auto curl = curl_easy_init(); curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://disease.sh/v2/all");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        auto res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
/*
    Output At the time of 6/30/2020:
    {"updated":1593569735145
    "cases":10585110
    "todayCases":7354
    "deaths":513913
    "todayDeaths":727
    "recovered":5795009
    "todayRecovered":4247
    "active":4276188
    "critical":57788
    "casesPerOneMillion":1358
    "deathsPerOneMillion":65.9
    "tests":240380732
    "testsPerOneMillion":30983.96
    "population":7758232404
    "oneCasePerPeople":0
    "oneDeathPerPeople":0
    "oneTestPerPeople":0
    "activePerOneMillion":551.18
    "recoveredPerOneMillion":746.95
    "criticalPerOneMillion":7.45
    "affectedCountries":215}
*/