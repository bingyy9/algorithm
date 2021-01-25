package service;

import com.atguigu.springcloud.entities.CommonResult;
import com.atguigu.springcloud.entities.Payment;
import org.springframework.stereotype.Component;

@Component
public class PaymentFallbackService implements PaymentFeignService{
    @Override
    public CommonResult<Payment> create(Payment payment) {
        return null;
    }

    @Override
    public CommonResult<Payment> getPaymentById(Long id) {
        return null;
    }

    @Override
    public String paymentInfo_OK(Integer id) {
        return "------fallback ok";
    }

    @Override
    public String paymentInfo_Timeout(Integer id) {
        return "------fallback timeout";
    }
}
