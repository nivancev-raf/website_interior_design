package nikolalukatrening.Notifikacioni_servis.dto;


import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.util.Map;

@Getter
@Setter
public class EmailMessageDto implements Serializable {
    private String email; // email of the user
    private String subject;
    private String message; // body of the email
    private String name; // name of the user


    public EmailMessageDto(String email, String subject, String message, String name) {
        this.email = email;
        this.subject = subject;
        this.message = message;
        this.name = name;
    }
}
