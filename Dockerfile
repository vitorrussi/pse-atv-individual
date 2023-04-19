FROM ubuntu:20.04

LABEL maintainer="vitorrussi"

ENV FREERTOS_PATH=/var/www/FreeRTOSv202212.01

RUN apt update
RUN apt install -y build-essential git
# && apt clean \
# && apt autoremove \
# && rm -rf /var/lib/apt/lists/*

COPY . /var/www
WORKDIR /var/www
