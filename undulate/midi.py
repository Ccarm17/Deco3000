import mido

msg = mido.Message('note_on', note=60)

out_port = mido.ports.BaseOutput('tweetland')
out_port.send(msg)
print(out_port)
