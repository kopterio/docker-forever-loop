FROM scratch

COPY forever-loop /

ENTRYPOINT ["/forever-loop"]
