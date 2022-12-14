ARG BOT_TOKEN
ARG HYPIXEL_API_KEY

FROM brainboxdotcc/dpp:latest

WORKDIR /usr/src/Bot108

COPY . .

RUN g++ -std=c++17 -O3 *.cpp *.hpp -o Bot108 -ldpp -pthread

ENTRYPOINT "/usr/src/Bot108/Bot108" "$BOT_TOKEN" "$HYPIXEL_API_KEY"