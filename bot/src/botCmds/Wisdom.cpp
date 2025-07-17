# include <Bot.hpp>

std::string Bot::enlightenMe(std::vector<std::string> &words) {
	(void)words;
	static const std::string wisdomWords[11] = {
		"'A veces la mejor decisión es no tomar ninguna decisión, que también es tomar una decisión' - Socrates",
		"'Cuanto peor, mejor para todos, y cuanto pero para todos, mejor, mejor para mí el suyo, beneficio político' - Friedrich Nietzsche",
		"'Los españoles son muy españoles y mucho españoles' - Zhuang Zi",
		"'Exportar es positivo porque vendes lo que produces' - Hegel",
		"'Es el vecino el que elige al alcalde, y es el alcalde el que quiere que sean los vecinos el alcalde' - Sun Tzu",
		"'Haré todo lo que pueda y un poco más de lo que pueda si es que eso es posible, y haré todo lo posible e incluso lo imposible si también lo imposible es posible' - Platón",
		"'Hay que fabricar máquinas que nos permitan seguir fabricando máquinas, porque lo que no va a hacer nunca una máquina es fabricar máquinas' - Locke",
		"'Me gustan los catalanes porque hacen cosas' - Lao-Tse",
		"'Somos sentimientos y tenemos seres humanos' - Mozi",
		"'La cerámica de Talavera no es cosa menor. Dicho de otra manera, es cosa mayor' - Descartes",
		"'It's very dificult todo esto' - Kong Fuz"
	};

	int code = rand() % 11;
	return wisdomWords[code];
}