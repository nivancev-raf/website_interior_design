
# Website Interior Design

**Website Interior Design** is a modern, responsive website created for showcasing interior design projects. This project primarily uses HTML, CSS, and JavaScript to deliver a seamless and visually appealing user experience. Additionally, the website includes a notification service built with Java Spring Boot, allowing users to send emails directly from the website. The notification service is containerized using Docker for easy deployment.

## Project Overview

The **Website Interior Design** project is designed to provide a platform where users can explore various interior design projects. The website is responsive and built with a mobile-first approach, ensuring compatibility across all devices. The project also includes a backend notification service that allows users to send inquiries or requests directly through the website.

### Key Features:
- **Responsive Design**: Fully responsive layout compatible with all devices.
- **Interactive UI**: Engaging user interface with smooth animations and transitions.
- **Java Spring Boot Notification Service**: Backend service for handling email notifications.
- **Dockerized Service**: The notification service is containerized using Docker for easy deployment and scaling.

## Technologies Used

- **Frontend**:
  - HTML5
  - CSS3
  - JavaScript (ES6+)
  - Bootstrap (optional)
- **Backend**:
  - Java Spring Boot
- **DevOps**:
  - Docker

## Getting Started

### Prerequisites
- Java Development Kit (JDK) 8 or higher
- Docker
- Web browser (Chrome, Firefox, etc.)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/nivancev-raf/website_interior_design.git
   ```

2. Navigate to the project directory:
   ```bash
   cd website_interior_design
   ```

3. **Run the Frontend**:
   - Open `index.html` in your preferred web browser to view the website.

4. **Run the Backend Notification Service**:
   - Ensure Docker is installed and running on your machine.
   - Build and start the Docker container for the Spring Boot service:
     ```bash
     docker build -t notification-service .
     docker run -d -p 8080:8080 notification-service
     ```

5. Access the website through your browser and test the email notification feature.

### Usage

- **Explore Projects**: Browse through the various interior design projects displayed on the website.
- **Send Inquiries**: Use the contact form to send an inquiry directly through the website. The email notification service will handle sending the email.
- **Responsive Experience**: The website is designed to be fully responsive, providing a seamless experience across all devices.

## Contact
For any questions or feedback, please reach out to nivancev02@gmail.com.
