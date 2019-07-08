FROM alpine:latest
RUN apk --update --no-cache add freeswitch \ 
  freeswitch-sample-config \ 
  freeswitch-sounds-en-us-callie-8000
CMD ["freeswitch"]
