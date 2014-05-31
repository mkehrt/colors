#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>

  /*
   * XYZ to sRGB conversion matrix
   */
static const double Mi[3][3]  = {{ 3.2406, -1.5372, -0.4986},
                            {-0.9689,  1.8758,  0.0415},
                                   { 0.0557, -0.2040,  1.0570}};
class xyYtoRGB {
  
  static std::vector<int> XYZtoRGB(double X, double Y, double Z) {
    std::vector<int>result (3);

    double x = X / 100.0;
    double y = Y / 100.0;
    double z = Z / 100.0;

    // [r g b] = [X Y Z][Mi]
    double r = (x * Mi[0][0]) + (y * Mi[0][1]) + (z * Mi[0][2]);
    double g = (x * Mi[1][0]) + (y * Mi[1][1]) + (z * Mi[1][2]);
    double b = (x * Mi[2][0]) + (y * Mi[2][1]) + (z * Mi[2][2]);

    // assume sRGB
    if (r > 0.0031308) {
      r = ((1.055 * pow(r, 1.0 / 2.4)) - 0.055);
    }
    else {
      r = (r * 12.92);
    }
    if (g > 0.0031308) {
      g = ((1.055 * pow(g, 1.0 / 2.4)) - 0.055);
    }
    else {
      g = (g * 12.92);
    }
    if (b > 0.0031308) {
      b = ((1.055 * pow(b, 1.0 / 2.4)) - 0.055);
    }
    else {
      b = (b * 12.92);
    }

    r = (r < 0) ? 0 : r;
    g = (g < 0) ? 0 : g;
    b = (b < 0) ? 0 : b;

    // convert 0..1 into 0..255
    result[0] = static_cast<int>(r * 255);
    result[1] = static_cast<int>(g * 255);
    result[2] = static_cast<int>(b * 255);

    return result;
  }

  static std::vector<double> xyYtoXYZ(double x, double y, double Y) {
    std::vector<double> result(3);

    if (y == 0) {
      result[0] = 0;
      result[1] = 0;
      result[2] = 0;
    }
    else {
      result[0] = (x * Y) / y;
      result[1] = Y;
      result[2] = ((1 - x - y) * Y) / y;
    }
    return result;
  }


public:
  static std::vector<int> f(double x, double y, double Y) {
    std::vector<double> xyz = xyYtoXYZ(x,y,Y);
    std::vector<int> rgb = XYZtoRGB(xyz[0], xyz[1], xyz[2]);
    return rgb;
  }
};

struct Munsell {
  struct Colorspace {
    class RGB;
    class xyY;
  };

  template<typename CS, typename T> struct Map {

    struct Triple {
      Triple() {}
      Triple(T x, T y, T z) : 
        p1(x), p2(y), p3(z)
      {}

      T p1, p2, p3;
    };

    typedef std::map<std::string, Triple> CTriple; 
    typedef std::map<std::string, CTriple> VCTriple;
    typedef std::map<std::string, VCTriple> Type;
  };

  static Map<Colorspace::xyY, double>::Type fromTSVFile(std::string filename) {
    Map<Colorspace::xyY, double>::Type result;

    char H[6], V[4], C[3];
    bzero(H, sizeof(H));
    bzero(V, sizeof(V));
    bzero(C, sizeof(C));
    double x, y, Y;

    FILE* file = fopen(filename.c_str(), "r");
    while (fscanf(file, "%s\t%s\t%s\t%lf\t%lf\t%lf", H, V, C, &x, &y, &Y) >= 0) {
      Map<Colorspace::xyY, double>::Triple t(x, y, Y);
      result[std::string(H)][std::string(V)][std::string(C)] = t;
    }

    return result;
  }

  static Map<Colorspace::RGB, int>::Type toRGB(Map<Colorspace::xyY, double>::Type m) {
    Map<Colorspace::RGB, int>::Type result;    

    for (auto hp : m) {
      for (auto vp : hp.second) {
        for (auto cp : vp.second) {
          std::string H = hp.first;
          std::string V = vp.first;
          std::string C = cp.first;
          auto t = cp.second;

          auto rgb = xyYtoRGB::f(t.p1, t.p2, t.p3);

          Map<Colorspace::RGB, int>::Triple tt(rgb[0], rgb[1], rgb[2]);
          result[H][V][C] = tt;
        }
      }
    }
    return result;
  }

  static std::string toJson(Map<Colorspace::RGB, int>::Type m)
  {
   std::stringstream ss;
   ss << "{";
   for (auto hp : m) {
    ss << "{" << "\"" << hp.first << "\":";
    for (auto vp : hp.second) {
      ss << "{" << "\"" << vp.first << "\":";
      for (auto cp : vp.second) {
        ss << "{" << "\"" << cp.first << "\":";

        auto t = cp.second;
        ss << "{" << "\"r\":" << t.p1 << ",\"g\":" << t.p2 << ",\"b\":" << t.p3 << "}";
      }
      ss << "},";
    }
    ss << "},";
  }
  ss << "}";

  return ss.str();
  }
      
};

int main(int a, char** aa) {
  auto xyY = Munsell::fromTSVFile(aa[1]);
  auto rgb = Munsell::toRGB(xyY);
  auto json = Munsell::toJson(rgb);
  printf("%s", json.c_str());
}
