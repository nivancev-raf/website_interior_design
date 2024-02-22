package nikolalukatrening.Notifikacioni_servis.controller;

import nikolalukatrening.Notifikacioni_servis.dto.EmailMessageDto;
import nikolalukatrening.Notifikacioni_servis.service.EmailService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/notifications")
public class NotificationController {
    private EmailService emailService;


    public NotificationController(EmailService emailService) {
        this.emailService = emailService;
    }

    // post endpoint for sending email
    @PostMapping("/sendEmail")
    public ResponseEntity<String> sendEmail(@RequestBody EmailMessageDto emailMessageDto) {
        // send email
        emailService.sendEmail(emailMessageDto);
        return ResponseEntity.ok("Email sent");
    }

}
