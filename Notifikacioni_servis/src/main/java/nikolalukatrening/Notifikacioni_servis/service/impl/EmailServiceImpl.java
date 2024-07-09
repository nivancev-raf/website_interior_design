package nikolalukatrening.Notifikacioni_servis.service.impl;

import nikolalukatrening.Notifikacioni_servis.dto.EmailMessageDto;
import nikolalukatrening.Notifikacioni_servis.service.EmailService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.stereotype.Service;

@Service
public class EmailServiceImpl implements EmailService {

    @Autowired
    private JavaMailSender emailSender;

    @Override
    public void sendEmail(EmailMessageDto emailMessage) {
        JavaMailSenderImpl mailSenderImpl = (JavaMailSenderImpl) emailSender;
        mailSenderImpl.getJavaMailProperties().put("mail.smtp.ssl.trust", "*");


        // treba pogledati kako za name
        SimpleMailMessage message = new SimpleMailMessage();
        message.setFrom("nivancev02@gmail.com");
        message.setTo(emailMessage.getEmail());
//        message.setTo("nivancev02@gmail.com");
//        System.out.println("email od:" + emailMessage.getEmail());
//        message.setFrom(emailMessage.getEmail());
        message.setSubject(emailMessage.getSubject());

        String mailMessage = "Vasa poruka je uspešno poslata sa sledećim sadržajem: " + "\n" + "\n" + emailMessage.getMessage() + "\n" + "\n" + "Ocekujte odgovor u najkracem mogucem roku. Hvala!";
        message.setText(mailMessage);
        mailSenderImpl.send(message);
    }
}


