FROM maven:3.8.5-openjdk-17 AS build
COPY . .
RUN mvn clean package -DskipTests

FROM openjdk:17.0.1-jdk-slim 
RUN apt-get update && apt-get install -y locales && locale-gen sr_RS.UTF-8
ENV LANG sr_RS.UTF-8
ENV LANGUAGE sr_RS:sr
ENV LC_ALL sr_RS.UTF-8
COPY --from=build /target/Notifikacioni_servis-0.0.1-SNAPSHOT.jar app.jar
EXPOSE 8081
ENTRYPOINT ["java", "-jar", "app.jar"]