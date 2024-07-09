package nikolalukatrening.Notifikacioni_servis.service;

import nikolalukatrening.Notifikacioni_servis.dto.EmailMessageDto;

public interface EmailService {

    public void sendEmail(EmailMessageDto emailMessage);

}
