/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#pragma once


#include <map>
#include <utility>
#include <vector>

#include "Eigen/Dense"

#include "IpIpoptCalculatedQuantities.hpp"
#include "IpIpoptData.hpp"
#include "IpOrigIpoptNLP.hpp"
#include "IpTNLP.hpp"
#include "IpTNLPAdapter.hpp"
#include "IpTypes.hpp"

namespace apollo {
namespace planning {

class CosThetaProbleminterface : public Ipopt::TNLP {
 public:
  explicit CosThetaProbleminterface(std::vector<Eigen::Vector2d> points,
                                    std::vector<double> lateral_bounds);

  virtual ~CosThetaProbleminterface() = default;

  void set_default_max_point_deviation(const double point_max_deviation);

  void set_start_point(const double x, const double y);

  void set_end_point(const double x, const double y);

  void set_weight_cos_included_angle(const double weight_cos_included_angle);

  void set_relax_end_constraint(const double relax);

  void get_optimization_results(std::vector<double>* ptr_x,
                                std::vector<double>* ptr_y) const;

  /** Method to return some info about the nlp */
  bool get_nlp_info(int& n, int& m, int& nnz_jac_g, int& nnz_h_lag,
                    IndexStyleEnum& index_style) override;

  /** Method to return the bounds for my problem */
  bool get_bounds_info(int n, double* x_l, double* x_u, int m, double* g_l,
                       double* g_u) override;

  /** Method to return the starting point for the algorithm */
  bool get_starting_point(int n, bool init_x, double* x, bool init_z,
                          double* z_L, double* z_U, int m, bool init_lambda,
                          double* lambda) override;

  /** Method to return the objective value */
  bool eval_f(int n, const double* x, bool new_x, double& obj_value) override;

  /** Method to return the gradient of the objective */
  bool eval_grad_f(int n, const double* x, bool new_x, double* grad_f) override;

  /** Method to return the constraint residuals */
  bool eval_g(int n, const double* x, bool new_x, int m, double* g) override;

  /** Method to return:
   *   1) The structure of the jacobian (if "values" is nullptr)
   *   2) The values of the jacobian (if "values" is not nullptr)
   */
  bool eval_jac_g(int n, const double* x, bool new_x, int m, int nele_jac,
                  int* iRow, int* jCol, double* values) override;

  /** Method to return:
   *   1) The structure of the hessian of the lagrangian (if "values" is nullptr)
   *   2) The values of the hessian of the lagrangian (if "values" is not nullptr)
   */
  bool eval_h(int n, const double* x, bool new_x, double obj_factor, int m,
              const double* lambda, bool new_lambda, int nele_hess, int* iRow,
              int* jCol, double* values) override;

  /** @name Solution Methods */
  /** This method is called when the algorithm is complete so the TNLP can
   * store/write the solution */
  void finalize_solution(Ipopt::SolverReturn status, int n, const double* x,
                         const double* z_L, const double* z_U, int m,
                         const double* g, const double* lambda,
                         double obj_value, const Ipopt::IpoptData* ip_data,
                         Ipopt::IpoptCalculatedQuantities* ip_cq) override;

 private:
  std::vector<Eigen::Vector2d> init_points_;

  std::vector<double> lateral_bounds_;

  std::vector<double> opt_x_;

  std::vector<double> opt_y_;

  std::size_t num_of_variables_ = 0;

  std::size_t num_of_constraints_ = 0;

  std::size_t nnz_jac_g_ = 0;

  std::size_t nnz_h_lag_ = 0;

  std::size_t num_of_points_ = 0;

  std::map<std::pair<std::size_t, std::size_t>, std::size_t> idx_map_;

  void hessian_strcuture();

  double default_max_point_deviation_ = 0.0;

  bool has_fixed_start_point_ = false;

  double start_x_ = 0.0;

  double start_y_ = 0.0;

  bool has_fixed_end_point_ = false;

  double end_x_ = 0.0;

  double end_y_ = 0.0;

  double relax_ = 0.2;

  double weight_cos_included_angle_ = 1.0;
};

}  // namespace planning
}  // namespace apollo
