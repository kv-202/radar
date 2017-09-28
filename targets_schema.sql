
CREATE TABLE targets (
        name CHAR(255) NOT NULL,
        time TIMESTAMP NOT NULL,
        x INT NOT NULL,
        y INT NOT NULL,
        z INT NOT NULL,
        state INT NOT NULL
);

CREATE TABLE last_targets (
        name CHAR(255) UNIQUE NOT NULL,
        time TIMESTAMP NOT NULL,
        x INT NOT NULL,
        y INT NOT NULL,
        z INT NOT NULL,
        state INT NOT NULL
);

