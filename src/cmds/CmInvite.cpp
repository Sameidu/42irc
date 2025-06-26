#include <Server.hpp>

// // INVITE <nickname> <channel>
// void Server::CmInvite(t_msg &msg, int fd) {
// 	// Esto solo lo pueden hacer los admins si lo intenta un usuario normal dar error
// 	/* 1.- Comprobar que los parametros sean como mucho 2
// 	 * 2.- Comprobar que el canal al que se invita exista
// 	 * 3.- Comprobar que el cliente que invita no esté en el canal
// 	 * 4.- Añadir al cliente invitado a la lista de invitados del canal
// 	 * 5.- Enviar mensaje al cliente invitado de que ha sido invitado al canal
// 	 * 6.- Si ya está, enviar error de que ya está en el canal
// 	 */
// 	if (msg.params.size() != 2) {
// 		answerClient(fd, ERR_NEEDMOREPARAMS, "Not enough parameters");
// 		return;
// 	}
// }
