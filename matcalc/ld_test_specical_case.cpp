
void process_original_C(uint8_t* data, uint8_t* mask, int width, int height)
{
    for (int i = 0; i < width; i++) 
    {
        int max_chl = 0;
        int max_val = -1;
        for (int j = 0; j < height; j++) 
        {
            int idx = i + j * width;
            if (data[idx] >= max_val) 
            {
                max_val = data[idx];
                max_chl = j;
            }
        }
        mask[i] = max_chl;
    }
}


void process_original_Neon(uint8_t* data, uint8_t* mask, int width, int height) 
{
    int width_NN = width >> 4;
    
    uint8x16_t data_c0_mask = vdupq_n_u8(0);
    uint8x16_t data_c1_mask = vdupq_n_u8(1);
    uint8x16_t data_c2_mask = vdupq_n_u8(2);
    uint8x16_t data_c3_mask = vdupq_n_u8(3);
    uint8x16_t data_c4_mask = vdupq_n_u8(4);
    uint8x16_t data_c5_mask = vdupq_n_u8(5);
    uint8x16_t data_c6_mask = vdupq_n_u8(6);
    uint8x16_t data_c7_mask = vdupq_n_u8(7);
    uint8x16_t data_c8_mask = vdupq_n_u8(8);
    
    for (int i = 0; i < width_NN; i++) 
    {
        int max_chl = 0;
        int max_val = -1;
        
        uint8_t*  data_c0 = data + i*16;
        uint8_t*  data_c1 = data_c0 + width;
        uint8_t*  data_c2 = data_c1 + width;
        uint8_t*  data_c3 = data_c2 + width;
        uint8_t*  data_c4 = data_c3 + width;
        uint8_t*  data_c5 = data_c4 + width;
        uint8_t*  data_c6 = data_c5 + width;
        uint8_t*  data_c7 = data_c6 + width;
        uint8_t*  data_c8 = data_c7 + width;
        
        uint8_t* mask_ = mask + i*16;
        
        uint8x16_t data_c0_V = vld1q_u8(data_c0);
        uint8x16_t data_c1_V = vld1q_u8(data_c1);
        uint8x16_t data_c2_V = vld1q_u8(data_c2);
        uint8x16_t data_c3_V = vld1q_u8(data_c3);
        uint8x16_t data_c4_V = vld1q_u8(data_c4);
        uint8x16_t data_c5_V = vld1q_u8(data_c5);
        uint8x16_t data_c6_V = vld1q_u8(data_c6);
        uint8x16_t data_c7_V = vld1q_u8(data_c7);
        uint8x16_t data_c8_V = vld1q_u8(data_c8);


        uint8x16_t V_tmp = vmaxq_u8(data_c0_V, data_c1_V);
        uint8x16_t V_result_mask = vcgtq_u8(data_c0_V, data_c1_V);
        uint8x16_t V_result = vbslq_u8(V_result_mask, data_c0_mask, data_c1_mask);
    
        V_result_mask = vcgtq_u8(V_tmp, data_c2_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c2_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c2_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c3_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c3_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c3_V);
        
        V_result_mask = vcgtq_u8(V_tmp, data_c4_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c4_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c4_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c5_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c5_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c5_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c6_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c6_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c6_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c7_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c7_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c7_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c8_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c8_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c8_V);

        vst1q_u8(mask_, V_result);
        
    }

}

static void ld_test()
{
    //srand(time(0));
    double t_start, t_cost;
    
    //int m = 9; //out_channels
    int height = 9;

    int width = 49152; //num_elem
    //int n = 48; //num_elem

    size_t buf_size = width * sizeof(uint8_t);

    //mask_OutData
    uint8_t* data = (uint8_t*)malloc(height*width);
    for (int i = 0; i < height*width; i++) {
        data[i] = (uint8_t)(rand()%127);
    }
    uint8_t* mask_c = (uint8_t*)malloc(width);
    uint8_t* mask_neon = (uint8_t*)malloc(width);
    memset(mask_c, 0, width);
    memset(mask_neon, 0, width);
    
    //-------------------------------------------
    // naive C impl
    //-------------------------------------------
    {
        t_start = pixel_get_current_time();
        process_original_C(data, mask_c, width, height);
        t_cost = pixel_get_current_time() - t_start;
        printf("c time: %.4lf \n", t_cost);
    }
    
    //-------------------------------------------
    // naive Neon impl
    //-------------------------------------------	
    {
        t_start = pixel_get_current_time();
        process_original_Neon(data, mask_neon, width, height);
        t_cost = pixel_get_current_time() - t_start;
        printf("neon time: %.4lf \n", t_cost);
    }
    int error = 0;
    printf(".......................inputdata........................\n");

    #if 1
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf("%d,  ", data[j + i*width]);
        }
        printf("\n");
    }
    printf("\n");
    #endif

    for(int i =0; i < width; i++) {
        if(mask_c[i]!=mask_neon[i]) {
            error++;
            printf("c: %d, neon: %d    i: %d \n", mask_c[i], mask_neon[i], i);
        }
    }
    printf("error number %d  \n", error);
}