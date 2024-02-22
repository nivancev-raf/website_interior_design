package nikolalukatrening.Notifikacioni_servis;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

@SpringBootApplication
@EnableScheduling
public class NotifikacioniServisApplication {

	public static void main(String[] args) {
		SpringApplication.run(NotifikacioniServisApplication.class, args);
	}

}
