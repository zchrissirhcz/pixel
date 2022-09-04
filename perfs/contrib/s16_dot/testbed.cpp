int32_t dot_simd(const short* v1, const short* v2, const int32_t len) {
#ifdef __ARM_NEON
    int32x4_t sum_vec = vdupq_n_s32(0);
    for(int k = 0; k < len; k += 4) {
        int16x4_t left_vec = vld1_s16(v1 + k);
        int16x4_t right_vec = vld1_s16(v2 + k);
        sum_vec = vmlal_s16(sum_vec, left_vec, right_vec);
    }

    int32x2_t r = vadd_s32(vget_high_s32(sum_vec), vget_low_s32(sum_vec));
    return vget_lane_s32(vpadd_s32(r, r), 0);
#endif // __ARM_NEON
}

int main() {
    int32_t len = 256;
    int loop_count = 1000000;
    double t_start, t_cost;

    t_start = pixel_get_current_time();
    for (int i=0; i<loop_count; i++) {
        dot_simd(v1, v2, len);
    }
    t_cost = pixel_get_current_time() - t_start;

    return 0;
}