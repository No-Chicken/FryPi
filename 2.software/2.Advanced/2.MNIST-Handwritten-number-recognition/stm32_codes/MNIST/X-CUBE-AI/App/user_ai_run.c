#include "user_ai_run.h"

extern struct ai_network_exec_ctx {
  ai_handle handle;
  ai_network_report report;
} net_exec_ctx[AI_MNETWORK_NUMBER];

uint8_t user_ai_run(const ai_float *in_data, ai_float *out_data)
{
    int idx = 0;
    int batch = 0;
    ai_buffer ai_input[AI_MNETWORK_IN_NUM];
    ai_buffer ai_output[AI_MNETWORK_OUT_NUM];

    ai_float input[1] = {0};  
    ai_float output[1] = {0};

    if (net_exec_ctx[idx].handle == AI_HANDLE_NULL)
    {
        printf("E: network handle is NULL\r\n");
        return -1;
    }

		ai_input[0] = net_exec_ctx[idx].report.inputs[0];
    ai_output[0] = net_exec_ctx[idx].report.outputs[0];
		
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    batch = ai_mnetwork_run(net_exec_ctx[idx].handle, ai_input, ai_output);
    if (batch != 1) {
      aiLogErr(ai_mnetwork_get_error(net_exec_ctx[idx].handle),
          "ai_mnetwork_run");
      return -2;
    }

    return 0;

}

uint8_t findMaxIndex(float arr[], uint8_t size) {
    if (size <= 0) {
        // 处理空数组或无效大小的情况
        return -1;
    }

    uint8_t maxIndex = 0;  // 假设最大值的索引为第一个元素的索引

    for (int i = 1; i < size; ++i) {
        // 检查当前元素是否大于当前最大值
        if (arr[i] > arr[maxIndex]) {
            maxIndex = i;  // 更新最大值的索引
        }
    }

    return maxIndex;
}


