package com.atguigu.springcloud.controller;

import com.atguigu.springcloud.entities.CommonResult;
import com.atguigu.springcloud.entities.Payment;
import com.atguigu.springcloud.service.PaymentService;
import com.netflix.hystrix.contrib.javanica.annotation.DefaultProperties;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.cloud.client.ServiceInstance;
import org.springframework.cloud.client.discovery.DiscoveryClient;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.List;
import java.util.concurrent.TimeUnit;

@RestController
@Slf4j

public class PaymentController {
    @Resource
    private PaymentService paymentService;

    @Value("${server.port}")
    private String serverPort;

    @Resource
    private DiscoveryClient discoveryClient;

    @PostMapping(value = "/payment/create")
    public CommonResult create(@RequestBody Payment payment){
        int result = paymentService.create(payment);
        log.info("*****插入结果： " + result);
        if(result > 0){
            return new CommonResult(200, " insert success", result + " serverPort " + serverPort);
        } else {
            return new CommonResult(444, "insert fail");
        }
    }

    @GetMapping(value = "/payment/get/{id}")
    public CommonResult getPaymentById(@PathVariable("id") Long id){
        Payment payment = paymentService.getPaymentById(id);
        log.info("*****查询结果： " + payment);
        if(payment != null){
            return new CommonResult(200, "query success", payment + " serverPort: " + serverPort);
        } else {
            return new CommonResult(444, "query no result for id: " + id);
        }
    }

    @GetMapping(value = "/payment/discovery")
    public Object discovery(){
        List<String> services = discoveryClient.getServices();
        for(String service: services){
            log.info("*****element: " + service);
        }

        List<ServiceInstance> instances = discoveryClient.getInstances("CLOUD-PAYMENT-SERVICE");
        for(ServiceInstance serviceInstance: instances){
            log.info(serviceInstance.getServiceId() + "\t" + serviceInstance.getHost() + "\t" + serviceInstance.getPort() + "\t" +  serviceInstance.getUri());
        }
        return discoveryClient;
    }

    @GetMapping(value = "/payment/lb")
    public String getPaymentLB(){
        return serverPort;
    }

    @GetMapping(value = "/payment/feign/timeout")
    public String paymentFeignTimeout(){
        try{
            TimeUnit.SECONDS.sleep(3);
        }catch (Exception e){

        }
        return serverPort;
    }

    @GetMapping("payment/hystrix/ok/{id}")
    public String paymentInfo_OK(Integer id){
        return paymentService.paymentInfo_OK(id);
    }

    @GetMapping("payment/hystrix/timeout/{id}")
    public String paymentInfo_Timeout(Integer id){
        return paymentService.paymentInfo_Timeout(id);
    }

}
