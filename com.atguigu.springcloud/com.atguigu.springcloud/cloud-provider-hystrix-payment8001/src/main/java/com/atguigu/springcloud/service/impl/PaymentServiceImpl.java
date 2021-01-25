package com.atguigu.springcloud.service.impl;

import cn.hutool.core.util.IdUtil;
import com.atguigu.springcloud.dao.PaymentDao;
import com.atguigu.springcloud.entities.Payment;
import com.atguigu.springcloud.service.PaymentService;
import com.netflix.hystrix.contrib.javanica.annotation.HystrixCommand;
import com.netflix.hystrix.contrib.javanica.annotation.HystrixProperty;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.PathVariable;

import javax.annotation.Resource;
import java.util.concurrent.TimeUnit;

@Service
public class PaymentServiceImpl implements PaymentService {

    @Resource
//    @Autowired
    private PaymentDao paymentDao;

    public int create(Payment payment){
        return paymentDao.create(payment);
    }

    public Payment getPaymentById(Long id) {
        return paymentDao.getPaymentById(id);
    }

    public String paymentInfo_OK(Integer id){
        return "线程池：" + Thread.currentThread().getName() + " paymentInfo OK";
    }

    @HystrixCommand(fallbackMethod = "paymentInfoHandler", commandProperties = {
            @HystrixProperty(name="execution.isolation.thread.timeoutInMilliseconds", value="3000")   //3s以内为正常，超过3s走fallbackMethod
    })
    public String paymentInfo_Timeout(Integer id){
        try {
            TimeUnit.SECONDS.sleep(5);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return "线程池：" + Thread.currentThread().getName() + " paymentInfo timeout";
    }

    public String paymentInfoHandler(Integer id){
        return "线程池：" + Thread.currentThread().getName() + " paymentInfo timeout fallback";
    }

    /////服务熔断
    @HystrixCommand(fallbackMethod = "paymentCircuitBreaker_fallback", commandProperties = {
            //是否开启断路器
            @HystrixProperty(name = "circuitBreaker.enabled", value = "true"),
            //请求次数
            @HystrixProperty(name = "circuitBreaker.requestVolumeThreshold", value = "10"),
            //时间窗口期
            @HystrixProperty(name = "circuitBreaker.sleepWindowInMilliseconds", value = "10000"),
            //失败了达到多少后跳闸
            @HystrixProperty(name = "circuitBreaker.errorThresholdPercentage", value = "60"),
            //10秒钟的时间内，大于10次访问，60%都是失败的, the circuit-breaker transitions from CLOSED to OPEN
            //配置参数都是基于HystrixCommandProperties.class
    })
    public String paymentCircuitBreaker(@PathVariable("id") Integer id){
        if(id < 0){
            throw new RuntimeException("id 不能负数");
        }
        String serialNumber = IdUtil.simpleUUID();
        return Thread.currentThread().getName() + "\t" + "调用成功， 流水号：" + serialNumber;
    }

    public String paymentCircuitBreaker_fallback(@PathVariable("id") Integer id){
        return "id不能负数，请稍后再试";
    }


}
