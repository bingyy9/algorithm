package service;

import com.atguigu.springcloud.entities.CommonResult;
import com.atguigu.springcloud.entities.Payment;
import org.apache.ibatis.annotations.Param;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.stereotype.Component;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

@Component
@FeignClient(value="CLOUD-PAYMENT-HYSTRIX-SERVICE", fallback = PaymentFallbackService.class)
public interface PaymentFeignService {
    @GetMapping("/consumer/payment/create")
    public CommonResult<Payment> create(Payment payment);
    @GetMapping("/consumer/payment/get/{id}")
    public CommonResult<Payment> getPaymentById(@PathVariable("id") Long id);

    @GetMapping("payment/hystrix/ok/{id}")
    public String paymentInfo_OK(Integer id);
    @GetMapping("payment/hystrix/timeout/{id}")
    public String paymentInfo_Timeout(Integer id);
}
