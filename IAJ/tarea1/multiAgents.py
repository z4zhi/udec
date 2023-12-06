# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
#
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent


class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [
            index for index in range(len(scores)) if scores[index] == bestScore
        ]
        chosenIndex = random.choice(bestIndices)  # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"

        # obtener distancia del fantasma mas cercano
        nearestGhost = 9999
        for ghostState in newGhostStates:
            distGhost = manhattanDistance(ghostState.getPosition(), newPos)
            nearestGhost = min(nearestGhost, distGhost)

        # obtener distancia de la comida mas cercana
        nearest = 9999
        for food in newFood.asList():
            distFood = manhattanDistance(food, newPos)
            nearest = min(nearest, distFood)

        # dar una recompensa mayor al estar mas cerca
        reward = 1.0 / nearest
        # si esta cerca de un fantasma penalizar
        if nearestGhost < 2:
            reward = -1000
        return successorGameState.getScore() + reward


def scoreEvaluationFunction(currentGameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()


class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn="scoreEvaluationFunction", depth="2"):
        self.index = 0  # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)


class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
          Returns a list of legal actions for an agent
          agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
          Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
          Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"

        # para minimax, se necesita el estado (nodo) y la profundidad deseada
        # 2 funciones recursivas similares, maximizar y minimizar

        # turno del pacman, quiere maximizar
        def maximize(depth, gameState):
            # caso base, llego a la profundidad, gano/perdio
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)

            best = -9999
            actions = gameState.getLegalActions(0)
            # para cada accion disponible, generar sucesor y hacer minimax con fantasmas
            for action in actions:
                successor = gameState.generateSuccessor(0, action)
                value = minimize(depth, 1, successor)  # fantasma quiere minimizar
                best = max(best, value)

            return best

        # turno de fantasmas, minimizar
        def minimize(depth, agentIndex, gameState):
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)

            best = 9999
            # como son varios fantasmas, hay que obtener el numero de agentes
            numAgents = gameState.getNumAgents()

            # si es el ultimo fantasma, hay que bajar en el arbol ya que sera el turno del pacman
            if agentIndex == numAgents - 1:
                depth += 1

            actions = gameState.getLegalActions(agentIndex)
            for action in actions:
                # generar sucesor de este fantasma
                successor = gameState.generateSuccessor(agentIndex, action)
                # ultimo fantasma, entonces sera el turno del pacman
                if agentIndex == numAgents - 1:
                    value = maximize(depth, successor)
                # sino, seguir con el siguiente fantasma (minimizar)
                else:
                    value = minimize(depth, agentIndex + 1, successor)
                best = min(best, value)

            return best

        # inicializacion, comienza con el pacman
        def optimalAction(gameState):
            optimal = None
            best = -9999

            for action in gameState.getLegalActions(0):
                successor = gameState.generateSuccessor(0, action)
                value = minimize(0, 1, successor)

                if value > best:
                    best = value
                    optimal = action

            return optimal

        return optimalAction(gameState)


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"

        def maximize(depth, gameState, alpha, beta):
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)

            best = -9999
            actions = gameState.getLegalActions(0)

            for action in actions:
                successor = gameState.generateSuccessor(0, action)
                value = minimize(depth, 1, successor, alpha, beta)
                best = max(best, value)

                # actualizar alfa con el mayor maximo
                alpha = max(alpha, best)
                # hay otra opcion menor (y mejor), luego no se continua
                if beta < alpha:
                    break

            return best

        def minimize(depth, agentIndex, gameState, alpha, beta):
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)

            best = 9999
            numAgents = gameState.getNumAgents()

            if agentIndex == numAgents - 1:
                depth += 1

            actions = gameState.getLegalActions(agentIndex)

            for action in actions:
                successor = gameState.generateSuccessor(agentIndex, action)

                if agentIndex == numAgents - 1:
                    value = maximize(depth, successor, alpha, beta)
                else:
                    value = minimize(depth, agentIndex + 1, successor, alpha, beta)
                best = min(best, value)

                beta = min(beta, best)
                if beta < alpha:
                    break

            return best

        def optimalAction(gameState):
            optimal = None
            best = -9999
            alpha = -9999
            beta = 9999

            for action in gameState.getLegalActions(0):
                successor = gameState.generateSuccessor(0, action)
                value = minimize(0, 1, successor, alpha, beta)

                if value > best:
                    best = value
                    optimal = action

                alpha = max(alpha, best)

            return optimal

        return optimalAction(gameState)


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
    Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"

        # expectimax es similar a minimax, cuenta con 2 funciones
        # maximize y expect, maximize es igual, expect es similiar a minimize

        def maximize(depth, gameState):
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)

            best = -9999
            actions = gameState.getLegalActions(0)

            for action in actions:
                successor = gameState.generateSuccessor(0, action)
                # el oponente no siempre tomara la decision optima
                value = expect(depth, 1, successor)
                best = max(best, value)

            return best

        # el oponente no siempre minimizara (tomara la desicion optima)
        # cambiar minimize por expect
        def expect(depth, agentIndex, gameState):
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)

            expected = 0.0
            actions = gameState.getLegalActions(agentIndex)

            for action in actions:
                successor = gameState.generateSuccessor(agentIndex, action)
                if agentIndex == gameState.getNumAgents() - 1:
                    value = maximize(depth + 1, successor)
                else:
                    value = expect(depth, agentIndex + 1, successor)

                # calcular valor promedio de una accion y se suma a expected
                # expected guarda el valor esperado/promedio de ganancias del agente
                expected += value / len(actions)

            return expected

        def optimalAction(gameState):
            optimal = None
            best = -9999

            for action in gameState.getLegalActions(0):
                successor = gameState.generateSuccessor(0, action)
                value = expect(0, 1, successor)

                if value > best:
                    best = value
                    optimal = action

            return optimal

        return optimalAction(gameState)


def betterEvaluationFunction(currentGameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"

    newPos = currentGameState.getPacmanPosition()
    newFood = currentGameState.getFood()
    newGhostStates = currentGameState.getGhostStates()
    newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

    # obtener distancia del fantasma mas cercano
    nearestGhost = 9999
    for i, ghostState in enumerate(newGhostStates):
        distGhost = manhattanDistance(ghostState.getPosition(), newPos)
        # si el fantasma esta asustado  y la distancia es menor a ese tiempo
        # el pacman se lo puede comer, entonces no es una amenaza. no se penaliza
        if newScaredTimes[i] > distGhost:
            continue
        nearestGhost = min(nearestGhost, distGhost)

    # obtener distancia de la comida mas cercana
    nearest = 9999
    for food in newFood.asList():
        distFood = manhattanDistance(food, newPos)
        nearest = min(nearest, distFood)

    # dar una recompensa mayor al estar mas cerca
    reward = 1.0 / (nearest + 1)
    # si esta cerca de un fantasma penalizar
    if nearestGhost < 1:
        reward = -1000

    return currentGameState.getScore() + reward


# Abbreviation
better = betterEvaluationFunction
