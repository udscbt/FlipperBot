from pygments.lexers.compiled import CppLexer
import pygments.token as pt

class ScheMoLexer(CppLexer):
  name = 'ScheMo'
  aliases = ['schemo']

  def get_tokens_unprocessed(self, text):
    schemo_cmd = None
    for index, token, value in CppLexer.get_tokens_unprocessed(self, text):
      if token is pt.Error and value == '@':
        schemo_cmd = index
      elif schemo_cmd is not None:
        yield index, pt.Keyword, '@'+value
        schemo_cmd = None
      elif token in pt.Comment and (
        value.startswith("/**") or
        value.startswith("/*!") or
        value.startswith("///") or
        value.startswith("//!")
      ):
        yield index, pt.String.Doc, value
        
      else:
        yield index, token, value
