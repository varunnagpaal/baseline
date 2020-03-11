#ifndef __SUM_ABS_DIFF_HPP
#define __SUM_ABS_DIFF_HPP
#include <cstdint>



#define ABS(x) ((x) >= 0) ? (x) : (-(x))





/* 
 * Version 0 - Singler work per tile 
 * In this version, each tile only performs one unit of work, i.e. calculating one
 * element of the result matrix. Based on the size of the result matrix, and the 
 * tile group dimensions, enough tile groups will be launched to populate the 
 * entire result matrix.
 */
int  __attribute__ ((noinline)) sum_abs_diff_single_work_per_tile (int *REF, int *FRAME, int *RES,
                                                                   uint32_t ref_height, uint32_t ref_width,
                                                                   uint32_t frame_height, uint32_t frame_width,
                                                                   uint32_t res_height, uint32_t res_width) {

        int start_y = __bsg_tile_group_id_y * bsg_tiles_Y + bsg_y;
        int end_y   = start_y + frame_height;
        int start_x = __bsg_tile_group_id_x * bsg_tiles_X + bsg_x;
        int end_x   = start_x + frame_width;


        int sad = 0;
        for (int y = start_y; y < end_y; y ++) {
                for (int x = start_x; x < end_x; x ++) {
                        sad += ABS ( (REF[y * ref_width + x] - FRAME[(y - start_y) * frame_width + (x - start_x)]) );
                }
        }

        RES[start_y * res_width + start_x] = sad;
        return 0;
}



/*
 * Version 1 - Multiple work per tile
 * In this version, each tile performs multiple units of work. The workload share 
 * of each tile group is defined by block_size_x/y and passed to the kernel. Each 
 * tile then performs the kernel in a loop, with each iteration belonging to a 
 * unit of work (or vitual thread).
 */
int  __attribute__ ((noinline)) sum_abs_diff_multiple_work_per_tile (int *REF, int *FRAME, int *RES,
                                                                     uint32_t ref_height, uint32_t ref_width,
                                                                     uint32_t frame_height, uint32_t frame_width,
                                                                     uint32_t res_height, uint32_t res_width,
                                                                     uint32_t block_size_y, uint32_t block_size_x) {


        int tile_group_start_y = __bsg_tile_group_id_y * block_size_y;
        int tile_group_end_y = tile_group_start_y + block_size_y;
        int tile_group_start_x = __bsg_tile_group_id_x * block_size_x;
        int tile_group_end_x = tile_group_start_x + block_size_x;
 

        for (int iter_y = tile_group_start_y + bsg_y; iter_y < tile_group_end_y; iter_y += bsg_tiles_Y) {
                for (int iter_x = tile_group_start_x + bsg_x; iter_x < tile_group_end_x; iter_x += bsg_tiles_X) {



                        int start_y = iter_y;
                        int end_y = iter_y + frame_height;
                        int start_x = iter_x;
                        int end_x = iter_x + frame_width;


                        int sad = 0;
                        for (int y = start_y; y < end_y; y ++) {
                                for (int x = start_x; x < end_x; x ++) {
                                        sad += ABS ( (REF [y * ref_width + x] - FRAME [(y - start_y) * frame_width + (x - start_x)]) );
                                }
                        }


                        RES [iter_y * res_width + iter_x] = sad;

                }
        }

        return 0;
}



/*
 * Version 2 - Frame dimensions fixed to 4x4
 * In this version frame dimensions are fixed to 4x4 and 
 * constant numbers are used instead of variables.
 */
int  __attribute__ ((noinline)) sum_abs_diff_fixed_frame_4x4 (int *REF, int *FRAME, int *RES,
                                                                     uint32_t ref_height, uint32_t ref_width,
                                                                     uint32_t res_height, uint32_t res_width,
                                                                     uint32_t block_size_y, uint32_t block_size_x) {


        int tile_group_start_y = __bsg_tile_group_id_y * block_size_y;
        int tile_group_end_y = tile_group_start_y + block_size_y;
        int tile_group_start_x = __bsg_tile_group_id_x * block_size_x;
        int tile_group_end_x = tile_group_start_x + block_size_x;
 

        for (int iter_y = tile_group_start_y + bsg_y; iter_y < tile_group_end_y; iter_y += bsg_tiles_Y) {
                for (int iter_x = tile_group_start_x + bsg_x; iter_x < tile_group_end_x; iter_x += bsg_tiles_X) {



                        int start_y = iter_y;
                        int end_y = iter_y + 4;
                        int start_x = iter_x;
                        int end_x = iter_x + 4;


                        int sad = 0;
                        for (int y = start_y; y < end_y; y ++) {
                                for (int x = start_x; x < end_x; x ++) {
                                        sad += ABS ( (REF [y * ref_width + x] - FRAME [(y - start_y) << 2 + (x - start_x)]) );
                                }
                        }


                        RES [iter_y * res_width + iter_x] = sad;

                }
        }

        return 0;
}



/*
 * Version 3 - Frame dimensions templatized
 * In this version, the frame dimensions are templatized in the kernel, instead 
 * of being passed in as an input argument to the kernel. This gives the compiler
 * the opportunity to optimize based on frame dimensions known at compiler time.
 */
template <int FRAME_HEIGHT, int FRAME_WIDTH>
int  __attribute__ ((noinline)) sum_abs_diff_fixed_frame (int *REF, int *FRAME, int *RES,
                                                          uint32_t ref_height, uint32_t ref_width,
                                                          uint32_t res_height, uint32_t res_width,
                                                          uint32_t block_size_y, uint32_t block_size_x) {


        int tile_group_start_y = __bsg_tile_group_id_y * block_size_y;
        int tile_group_end_y = tile_group_start_y + block_size_y;
        int tile_group_start_x = __bsg_tile_group_id_x * block_size_x;
        int tile_group_end_x = tile_group_start_x + block_size_x;
 

        for (int iter_y = tile_group_start_y + bsg_y; iter_y < tile_group_end_y; iter_y += bsg_tiles_Y) {
                for (int iter_x = tile_group_start_x + bsg_x; iter_x < tile_group_end_x; iter_x += bsg_tiles_X) {



                        int start_y = iter_y;
                        int end_y = iter_y + FRAME_HEIGHT;
                        int start_x = iter_x;
                        int end_x = iter_x + FRAME_WIDTH;


                        int sad = 0;
                        for (int y = start_y; y < end_y; y ++) {
                                for (int x = start_x; x < end_x; x ++) {
                                        sad += ABS ( (REF [y * ref_width + x] - FRAME [(y - start_y) * FRAME_WIDTH + (x - start_x)]) );
                                }
                        }


                        RES [iter_y * res_width + iter_x] = sad;

                }
        }

        return 0;
}





#endif //__SUM_ABS_DIFF_HPP
