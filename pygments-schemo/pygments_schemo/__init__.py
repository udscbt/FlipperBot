from pygments.lexers.compiled import CppLexer
from pygments.token import Name, Keyword, Error

class ScheMoLexer(CppLexer):
  name = 'ScheMo'
  aliases = ['schemo']

  def get_tokens_unprocessed(self, text):
    schemo_cmd = None
    for index, token, value in CppLexer.get_tokens_unprocessed(self, text):
      if token is Error and value == '@':
        schemo_cmd = index
        continue
      elif schemo_cmd is not None:
        yield index, Keyword, '@'+value
        schemo_cmd = None
      else:
        yield index, token, value
