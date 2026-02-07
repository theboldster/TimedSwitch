// Library to calculate Dawn, Sun-up, Sundown and dusk

// Bürgerliche Zeiten, Standort Herrenberg

#include <math.h>
#include <stdio.h>
#include <time.h>

#define DEG2RAD(x) ((x) * M_PI / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

// Koordinaten Herrenberg
#define LAT 48.595
#define LON 8.868

typedef struct {
    double sunrise;
    double sunset;
    double dawn;
    double dusk;
} SunTimes;

// Tag des Jahres berechnen
int day_of_year(int y, int m, int d) {
    struct tm t = {0};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    mktime(&t);
    return t.tm_yday + 1;
}

// EU-Sommerzeit prüfen (letzter Sonntag im März bis letzter Sonntag im Oktober)
int is_dst_eu(int y, int m, int d) {
    struct tm t = {0};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    mktime(&t);

    int wday = t.tm_wday; // 0=Sonntag
    int lastSunday;

    if (m == 3) { // März
        lastSunday = 31 - ((wday + 31 - d) % 7);
        return d >= lastSunday;
    }
    if (m == 10) { // Oktober
        lastSunday = 31 - ((wday + 31 - d) % 7);
        return d < lastSunday;
    }
    return (m > 3 && m < 10);
}

// Gleichung der Zeit (in Minuten)
double equation_of_time(int N) {
    double B = DEG2RAD((360.0 / 365.0) * (N - 81));
    return 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
}

// Sonnendeklination
double declination(int N) {
    return DEG2RAD(23.44) * sin(DEG2RAD((360.0 / 365.0) * (N - 81)));
}

// Stundenwinkel
double hour_angle(double lat, double decl, double h) {
    double num = sin(h) - sin(lat) * sin(decl);
    double den = cos(lat) * cos(decl);
    return acos(num / den);
}

// Hauptfunktion
SunTimes calc_suntimes(int year, int month, int day) {
    SunTimes st;

    int N = day_of_year(year, month, day);
    double lat = DEG2RAD(LAT);
    double decl = declination(N);

    // Höhenwinkel
    double h_sun = DEG2RAD(-0.833); // Sonnenauf/untergang
    double h_twilight = DEG2RAD(-6.0); // bürgerliche Dämmerung

    double H_sun = hour_angle(lat, decl, h_sun);
    double H_twilight = hour_angle(lat, decl, h_twilight);

    double t_sun = RAD2DEG(H_sun) / 15.0;
    double t_twilight = RAD2DEG(H_twilight) / 15.0;

    // Gleichung der Zeit
    double eot = equation_of_time(N);

    // Zeitzone MEZ = UTC+1
    double tz = 1.0 + (is_dst_eu(year, month, day) ? 1.0 : 0.0);

    // wahre Sonnenmittagzeit
    double solar_noon = 12.0 + tz - (LON / 15.0) - (eot / 60.0);

    st.sunrise = solar_noon - t_sun;
    st.sunset  = solar_noon + t_sun;
    st.dawn    = solar_noon - t_twilight;
    st.dusk    = solar_noon + t_twilight;

    return st;
}

// Hilfsfunktion zur Ausgabe
void print_time(double t) {
    int h = (int)t;
    int m = (int)((t - h) * 60);
    int s = (int)((t - h - m/60.0) * 3600);

    if (h < 10) Serial.print(0);
    Serial.print(h);
    Serial.print(":");
    if (m < 10) Serial.print(0);
    Serial.print(m);
    Serial.print(":");
    if (s < 10) Serial.print(0);
    Serial.print(s);
    // printf("%02d:%02d:%02d", h, m, s);
}

/*
// Beispiel main
int main() {
    SunTimes st = calc_suntimes(2026, 2, 7);

    Serial.print("Daemmerung Beginn: "); print_time(st.dawn); Serial.print("\n");
    Serial.print("Sonnenaufgang: "); print_time(st.sunrise); Serial.print("\n");
    Serial.print("Sonnenuntergang: "); print_time(st.sunset); Serial.print("\n");
    Serial.print("Daemmerung Ende: "); print_time(st.dusk); Serial.print("\n");

    return 0;
}

*/