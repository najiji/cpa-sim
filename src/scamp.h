//
// Created by najiji on 19.05.18.
//

#ifndef SCAMP_H
#define SCAMP_H

#include "opencv2/opencv.hpp"
#include "sim.h"
#include "scamp_def.h"
#include "scamp_access.h"

#define SCAMP_WIDTH 256
#define SCAMP_HEIGHT 256

// Error model
#define SIGNAL_NON_LINEARITY 0.07
#define GAUSSIAN_NOISE_STDDEV 0.09
#define PATTERN_NOISE_STDDEV 0.05


using namespace cv;
using namespace SCAMP;

class Scamp {

private:
    Sim* sim_ptr;

    const UMat A;
    const UMat B;
    const UMat C;
    const UMat D;
    const UMat E;
    const UMat F;
    const UMat NEWS;

    const UMat _AWORK;

    const UMat R0;
    const UMat R1;
    const UMat R2;
    const UMat R3;
    const UMat R4;
    const UMat R5;
    const UMat R6;
    const UMat R7;
    const UMat R8;
    const UMat R9;
    const UMat R10;
    const UMat R11;
    const UMat R12;
    const UMat FLAG;

    const UMat _DWORK;

    const UMat FIXED_PATTERN_NOISE;

    bool noise_enabled = false;
    void add_noise(const UMat& reg) const;
    const UMat &analog(areg_t a) const;
    const UMat &digital(dreg_t a) const;


public:
    Scamp(Sim *simulator);

    void make_global();
    ScampAccess register_reference_analog(areg_t reg);
    ScampAccess register_reference_digital(dreg_t reg);

    void perform_operation_analog(opcode_t op, areg_t r1, areg_t r2, areg_t r3) const;
    void perform_operation_digital(opcode_t op, dreg_t r1, dreg_t r2, dreg_t r3) const;
    void perform_operation_analog_io(opcode_t op, areg_t r, int a) const;
    void perform_operation_digital_io(opcode_t op, dreg_t r, int a, int b, int c, int d) const;

    void perform_operation_analog_transform(areg_t r1, areg_t r2, int x, int y, int s, bool n) const;
    void perform_operation_digital_transform(dreg_t r1, dreg_t r2, int x, int y) const;

    void enable_noise();
    void disable_noise();
};


extern Scamp *scamp_ptr;


inline void rpix(areg_t target) {
    scamp_ptr->perform_operation_analog(RPIX, target, (areg_t)0, (areg_t)0);
}

inline void res(areg_t target) {
    scamp_ptr->perform_operation_analog(RES, target, (areg_t)0, (areg_t)0);
}

inline void add(areg_t target, areg_t source1, areg_t source2) {
    scamp_ptr->perform_operation_analog(ADD, target, source1, source2);
}

inline void addneg(areg_t target, areg_t source1, areg_t source2) {
    scamp_ptr->perform_operation_analog(ADDNEG, target, source1, source2);
}

inline void sub(areg_t target, areg_t source1, areg_t source2) {
    scamp_ptr->perform_operation_analog(SUB, target, source1, source2);
}

inline void north(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(NORTH, target, source, (areg_t)0);
}

inline void east(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(EAST, target, source, (areg_t)0);
}

inline void south(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(SOUTH, target, source, (areg_t)0);
}

inline void west(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(WEST, target, source, (areg_t)0);
}

inline void mov(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(MOV, target, source, (areg_t)0);
}

inline void div2(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(DIV2, target, source, (areg_t)0);
}

inline void neg(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(NEG, target, source, (areg_t)0);
}

inline void inv(areg_t target, areg_t source) {
    scamp_ptr->perform_operation_analog(INV, target, source, (areg_t)0);
}


inline void d_nor(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(NOT, target, source, (dreg_t)0);
}

inline void d_nor(dreg_t target, dreg_t source1, dreg_t source2) {
    scamp_ptr->perform_operation_digital(NOR, target, source1, source2);
}

inline void d_mov(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(MOV, target, source, (dreg_t)0);
}

inline void d_north(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(NORTH, target, source, (dreg_t)0);
}

inline void d_east(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(EAST, target, source, (dreg_t)0);
}

inline void d_south(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(SOUTH, target, source, (dreg_t)0);
}

inline void d_west(dreg_t target, dreg_t source) {
    scamp_ptr->perform_operation_digital(WEST, target, source, (dreg_t)0);
}

inline void where(areg_t a) {
    scamp_ptr->perform_operation_analog(WHERE, a, (areg_t)0, (areg_t)0);
}

inline void d_where(dreg_t a) {
    scamp_ptr->perform_operation_digital(WHERE, a, (dreg_t)0, (dreg_t)0);
}

inline void all() {
    scamp_ptr->perform_operation_digital(ALL, (dreg_t)0, (dreg_t)0, (dreg_t)0);
}


inline void d_select_pattern(dreg_t target, addr_t a1, addr_t a2, addr_t a3, addr_t a4) {
    scamp_ptr->perform_operation_digital_io(SELECT_PATTERN, target, a1, a2, a3, a4);
}


inline void in(areg_t target, dac_t value) {
    scamp_ptr->perform_operation_analog_io(IN, target, value);
}

inline void d_load(dreg_t target, bool value) {
    scamp_ptr->perform_operation_digital_io(IN, target, value?255:0, (dreg_t)0, (dreg_t)0, (dreg_t)0);
}


inline void _transform(areg_t target, areg_t source, int x_shift, int y_shift, int scale, bool inv) {
    scamp_ptr->perform_operation_analog_transform(target, source, x_shift, y_shift, scale, inv);
}


inline void _d_transform(dreg_t target, dreg_t source, int x_shift, int y_shift) {
    scamp_ptr->perform_operation_digital_transform(target, source, x_shift, y_shift);
}


#endif //CPA_SIM_CPA_H
