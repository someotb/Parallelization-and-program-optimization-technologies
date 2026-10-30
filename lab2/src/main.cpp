#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

void write_file(const std::string &path, int threads, double time) {
  std::ofstream out(path, std::ios::app);
  if (!out.is_open()) {
    std::cerr << "Failed to open " << path << "\n";
    return;
  }
  out << threads << "," << time << "\n";
}

void output(std::vector<std::vector<double>> &matrix,
            std::vector<double> &vector) {
  std::cout << "Matrix:\n";
  for (const auto &row : matrix) {
    for (auto el : row)
      std::cout << el << " ";
    std::cout << "\n";
  }

  std::cout << "\nVector: ";
  for (const auto &e : vector)
    std::cout << e << " ";

  std::cout << "\n";
}

std::vector<double> band_to_string(size_t k, size_t rows, size_t cols,
                                   std::vector<std::vector<double>> &matrix,
                                   std::vector<double> &vector) {
  std::vector<double> result(rows, 0);

  for (size_t band_start = 0; band_start < rows; band_start += k) {
    size_t band_end = std::min(rows, band_start + k);
    double res = 0;
#pragma omp parallel for reduction(+ : res)
    for (size_t row = band_start; row < band_end; ++row) {
      for (size_t col = 0; col < cols; ++col)
        res += matrix[row][col] * vector[col];
      result[row] = res;
      res = 0.0;
    }
  }
  return result;
}

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0.0, 100.0);

  size_t cols = 0;
  size_t rows = 0;

  std::cout << "Matrix:\nEnter num of columns(vector size too): ";
  std::cin >> cols;

  std::cout << "Enter num of rows: ";
  std::cin >> rows;

  std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0));
  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      matrix[i][j] = dis(gen);

  std::vector<double> vector(cols, 0);
  for (size_t i = 0; i < cols; ++i)
    vector[i] = dis(gen);

  // output(matrix, vector);

  size_t k = 0;
  std::cout << "Enter band width: ";
  std::cin >> k;
  if (k == 0)
    k = 1;

  std::string path = "data/result.csv";
  std::ofstream(path, std::ios::trunc);
  int threads[6] = {1, 2, 4, 16, 32, 64};
  for (auto thread : threads) {
    omp_set_num_threads(thread);
    double t0 = omp_get_wtime();
    auto result = band_to_string(k, rows, cols, matrix, vector);
    double t1 = omp_get_wtime();
    write_file(path, thread, t1 - t0);
  }

  return 0;
}