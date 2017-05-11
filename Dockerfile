# --- builder container ---

FROM alpine:3.4

COPY forever-http-loop.c /

RUN apk update
RUN apk add gcc musl-dev
RUN gcc -Wall -o forever-http-loop forever-http-loop.c -static
RUN strip forever-http-loop

# --- actual docker image recipe ---

FROM scratch

COPY --from=0 /forever-http-loop /

ENTRYPOINT ["/forever-http-loop"]
