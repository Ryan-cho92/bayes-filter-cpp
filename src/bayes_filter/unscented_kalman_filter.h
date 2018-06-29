/*
 * unscented_kalman_filter.h
 *
 *  Created on: 18 May 2018
 *      Author: Fabian Meyer
 */

#ifndef BFCPP_UNSCENTED_KALMAN_FILTER_H_
#define BFCPP_UNSCENTED_KALMAN_FILTER_H_

#include "bayes_filter/bayes_filter.h"
#include "bayes_filter/unscented_transform.h"

namespace bf
{
    /** Implementation of a Unscented Kalman Filter.*/
    class UnscentedKalmanFilter: public BayesFilter
    {
    public:
        typedef std::function<Eigen::VectorXd(const Eigen::VectorXd &)>
        NormalizeFunc;
    private:
        UnscentedTransform unscentTrans_;

        NormalizeFunc normState_;
        NormalizeFunc normObs_;

        Eigen::VectorXd state_;
        Eigen::MatrixXd cov_;

        Eigen::VectorXd calcMean(const SigmaPoints &sigma) const;
        Eigen::VectorXd calcCovariance(
            const SigmaPoints &sigma,
            const Eigen::VectorXd &mean) const;
        Eigen::VectorXd calcCrossCovariance(
            const SigmaPoints &sigmaA,
            const Eigen::VectorXd &meanA,
            const SigmaPoints &sigmaB,
            const Eigen::VectorXd &meanB) const;

    public:
        UnscentedKalmanFilter();
        UnscentedKalmanFilter(MotionModel *mm, SensorModel *sm);
        ~UnscentedKalmanFilter();

        /** Set the normalization function for state vectors.
         *  @param normalize normalization function */
        void setNormalizeState(const NormalizeFunc &normalize);

        /** Set the normalization function for observation vectors.
         *  @param normalize normalization function */
        void setNormalizeObservation(const NormalizeFunc &normalize);

        StateEstimate getEstimate() const override;

        void init(const Eigen::VectorXd &state,
                  const Eigen::MatrixXd &cov) override;
        void predict(const Eigen::VectorXd &controls,
                     const Eigen::MatrixXd &observations,
                     const Eigen::MatrixXd &motionCov) override;
        void correct(const Eigen::MatrixXd &observations,
                     const Eigen::MatrixXd &sensorCov) override;
    };
}

#endif
