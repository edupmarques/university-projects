# FilmHub — Movie Recommendation System

> University project developed for the **Automated Software Engineering** course.
> ⚠️ This repository contains only the **traditional development** implementation. The AI-assisted version is maintained in a separate repository.
> Group repository: [filmhub-project](https://github.com/martinresplandy/filmhub-project)

## About the Project

FilmHub is a movie recommendation web application where users can register, browse films,
rate them, and receive personalized recommendations. The project was built by a team of 5
students with the goal of comparing traditional software development against an AI-assisted
workflow, while also setting up a full CI/CD pipeline.

## Tech Stack

| Layer     | Technology                        |
|-----------|-----------------------------------|
| Backend   | Python, Django, Django REST Framework |
| Frontend  | React, React Router, Axios        |
| Database  | SQLite (dev) / configurable       |
| DevOps    | Docker, Docker Compose, GitHub Actions |

## Key Features

- User registration, login, and profile management
- Movie catalog browsing with details (title, genre, description, rating)
- Search by title, director, or genre
- User ratings (1–5 stars)
- Personalized recommendation engine

## Project Structure

filmhub-project/
├── api/              # Django REST API
├── filmhub/          # Django project settings
├── frontend/         # React application
├── .github/workflows # CI/CD pipeline (GitHub Actions)
├── Dockerfile
├── docker-compose.yml
└── Makefile

## Running Locally

### Backend

```bash
python3 -m venv .venv
source .venv/bin/activate
make backend
```

### Frontend

```bash
cd frontend
npm install
npm start
```

## CI/CD Pipeline

The project uses **GitHub Actions** for automated builds and testing on every push.
Docker is used for containerized deployment.

## Course Context

This project was part of the **Automated Software Engineering** course at the University
of Coimbra. The central goal was to compare two development workflows:

- **Traditional** — manual coding, manual testing, handwritten documentation
- **AI-assisted** — code generation, test generation, and documentation with LLMs

Both approaches were applied to significant parts of the system and compared in the
final report.

## My Contributions

- **Frontend** — Led the frontend development, building the React application
  including routing, API integration with Axios, and UI components.
- **CI/CD Pipeline** — Set up the first version of the GitHub Actions pipeline,
  configuring automated builds and workflow structure.
- **Testing** — Wrote the test suite for the project, covering unit and
  integration tests to ensure system reliability.
- **Final Report** — Actively contributed to the final report, including the
  comparison between traditional and AI-assisted development workflows.
