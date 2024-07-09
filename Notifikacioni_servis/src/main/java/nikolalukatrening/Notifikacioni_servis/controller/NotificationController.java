package nikolalukatrening.Notifikacioni_servis.controller;

import nikolalukatrening.Notifikacioni_servis.dto.EmailMessageDto;
import nikolalukatrening.Notifikacioni_servis.service.EmailService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
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
    public ResponseEntity<Map<String, String>> sendEmail(@RequestBody EmailMessageDto emailMessageDto) {
        // send email
        emailService.sendEmail(emailMessageDto);
        Map<String, String> response = new HashMap<>();
        response.put("message", "Email sent");
        return ResponseEntity.ok(response);
    }

}
