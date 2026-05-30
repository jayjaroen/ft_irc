*Project ft_irc has been created as part of the 42 common core curriculum by a collaborative effort of gyeepach, psenalia, and jjaroens.*

### INTRODUCTION
___
**Internet Relay Chat (IRC)** is a communication system developed into a platform that allows users to perform network communication remotely. This implementation has been developed over several years, resulting in solid structures, forms, and protocols that became universal to all IRC uses across all users and developers. Well established and intended for mass distributed machines, IRC has become the backbone for many chat platforms to date.

### 42 ITERATION
___
According to the subject provided by 42 for this project, this iteration of IRC is to achieve the following:
- A singular server (typically localhost) that supports the entire IRC program.
- The server must be able to handle multiple operations simultaneously, as requests from multiple users may be sent to the server at once.
- Users can connect to the server as clients, where all of the server-to-client communication are done via TCP/IP in either v4 or v6.
- Users can create and/or join chatrooms known as channels, in which can interact with other clients within the same channel via chat messages.
- The client who first created the channel is granted priviledge as an operator, gaining access to modify the channel.
- Once joined the channel, clients can also leave at their discretion. The same also applies for disconnecting from the IRC server.
- Users can connect to this server and able to use all of its capabilities mentioned above through **at least** one IRC client program of choice.
- Only the expected errors and procedures of a typical IRC protocols are allowed, and the protocol numbers of said error reports **must** align with the universal IRC protocol rules.

### PROJECT RESULTS
___
Our program is an IRC server that is compatible with **irssi** client program that allows up to 10 clients to join while still performing smoothly at its peak.
### Server Side
Our Makefile is used to compile the server program "ft_irc" under the bash command:
> c++ -Wall -Wextra -Werror -std=c++98 -g3

After which, the server can be setup under the command:
> ./ft_irc [port] [pass]

Where the first parameter indicates port serial number and the second parameter indicates the password for joining/connecting to the server. Both parameters are expected to be written in strictly four-digit numbers.

Once properly executed, the terminal used will be dedicated to the server as the program will show
> Server: listening on port: [port]

while any activity the server receives and acknowledges during the server runtime will be displayed after the line above.
### Client Side
Clients can connect to the server via two commands; netcat (nc) or irssi. For project evaluation, both methods must be used for testing. Below is the commands for netcat method:
> nc -C localhost [port]

After which, netcat clients would have to manually authenticate in order to verify connection and gain full access to the IRC server. This can be done by:
- First, type in the password of the server via PASS command, followed by the four-digit password.
	> PASS [password]
- Then, set display nickname on the server via NICK command, followed by the desired display name.
	> NICK [nickname]
- Finally, set username via USER command, followed by username, 0, *, and real name (real name would not be mentioned nor displayed to any other clients).
	> USER [username] 0 * [realname]

Once all typed in correctly, the server would reply back to the client with the following text
> :ircserver 001 [nickname] :Welcome to the IRC server, [nickname]!

along with other details, signalling successful authentication.

For irssi, clients can directly launch it if irssi has already been installed. Once inside, connect and authenticate to the server via CONNECT command with parameters as shown below:
> /connect localhost [port] [pass]

When port and password number is typed in correctly, irssi would established connection to the server and automatically authenticates the client.

### IMPLEMENTED COMMANDS
Below is the table listing all available commands implemented in this project and how it is supposed to be used:
| Command | Function | Usage |
| ------- | -------- | ----- |
| CONNECT | For connecting to the server | CONNECT [host] [port] [pass] |
| PASS    | Authenticate connection via server password | PASS [pass] |
| USER    | Set up username of the client | USER [username] 0 * [realname] |
| NICK    | Set up display name of the client | NICK [nickname] |
| CAP     | View and negotiate capabilities provided by the IRC server | CAP [subcommands] |
|| Subcommand for listing all available capabilities provided by the server | CAP LS |
|| Subcommand for listing all capabilities the user has been granted | CAP LIST |
|| Subcommand for the user to request for specified available capability | CAP REQ [capability] |
|| Subcommand for ending capability negotiations | CAP END |
| QUIT    | For disconnecting from the server | QUIT |
| PING    | For fetching/checking connection to the server | PING |
| JOIN    | For joining the specified channel, or create a new one if the channel doesn't exist. Can add keys as a password to the private channel. Can join/create multiple channels and keys by separating each with comma ",". | JOIN [#channel1,#channel2,...] [key1,key2,...(optional)] |
| PART    | For leaving the specified channel | PART |
| PRIVMSG | For sending message to the specified target, which can be client or channel | PRIVMSG [target] : [text] |
| MODE    | For channel operator to modify the channel. This project supports k, l, t, o, i mode flags | MODE [channel] [+/-][flag] [input(conditional)] |
| TOPIC   | Set up the topic of conversation for the channel | TOPIC [channel] [topic] |
| INVITE  | For inviting target client(s) not in the channel to join | INVITE [nickname] [channel] |
| KICK    | For channel operator to kick the specified client out of the channel | KICK [channel] [nickname] |
| HELP    | For displaying the function of specified command | HELP [command] |
> *NOTE that in irssi, every command must instead be typed as /lowercase instead of typical UPPERCASE format*