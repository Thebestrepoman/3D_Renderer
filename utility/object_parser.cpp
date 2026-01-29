#include "object_parser.h"

namespace renderer {

std::vector<Triangle> ExtractFromObj(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Vec3> points;
    std::vector<std::array<int, 3>> faces;
    std::string line;
    std::string cur;
    Vec3 sum_points = Vec3::Zero();
    int num_points = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> cur;
        if (cur == "v") {
            double v1, v2, v3;
            ss >> v1 >> v2 >> v3;
            points.emplace_back(v1, v2, v3);
            sum_points += points.back();
            num_points++;
        }
        if (cur == "f") {
            int a, b, c;
            ss >> a >> b >> c;
            faces.push_back({a, b, c});
        }
    }
    sum_points /= num_points;
    std::vector<Vec3> normals(points.size(), Vec3::Zero());
    for (size_t i = 0; i < faces.size(); i++) {
        Vec3 fnorm = CalculateNormalForThreeVectors(points[faces[i][0] - 1], points[faces[i][1] - 1], points[faces[i][2] - 1]);
        Vec3 center = (points[faces[i][0] - 1] + points[faces[i][1] - 1] + points[faces[i][2] - 1]) / 3;
        if ((center - sum_points).dot(fnorm) < 0) {
            fnorm = fnorm;
        }
        normals[faces[i][0] - 1] += fnorm;
        normals[faces[i][1] - 1] += fnorm;
        normals[faces[i][2] - 1] += fnorm;
    }
    for (size_t i = 0; i < normals.size(); i++) {
        normals[i].normalize();
    }
    std::vector<Triangle> triangles;
    for (size_t i = 0; i < faces.size(); i++) {
        triangles.emplace_back(Vertex(points[faces[i][0] - 1], default_colour_k, normals[faces[i][0] - 1]),
                               Vertex(points[faces[i][1] - 1], default_colour_k, normals[faces[i][1] - 1]),
                               Vertex(points[faces[i][2] - 1], default_colour_k, normals[faces[i][2] - 1]));
    }
    return triangles;
}
}  // namespace renderer
