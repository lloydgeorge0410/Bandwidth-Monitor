import uvicorn

from app.config.settings import settings
from app.utils.constants import MODE

if __name__ == "__main__":
    uvicorn.run(
        "app.app:app",
        reload=settings.mode == MODE.DEV,
        reload_dirs="app" if settings.mode == MODE.DEV else None,
        port=settings.port,
        host=settings.host,
        log_config=settings.logging_config,
        workers=4 if settings.mode == MODE.PROD else None,
    )
